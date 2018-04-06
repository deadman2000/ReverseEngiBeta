import io
from ctypes import c_uint8, c_ushort, LittleEndianStructure, BigEndianStructure, sizeof
from enum import Enum, IntEnum
from struct import Struct


class EEnum(IntEnum):
    @classmethod
    def find_value(cls, value):
        return any(value == item.value for item in cls)

    @classmethod
    def from_value(cls, value):
        if cls.find_value(value):
            return cls(value)
        return value


class ByteOrder(Enum):
    BE = 0,
    LE = 1


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)
        self.__dict__ = self


class Element(object):
    byteorder = None
    _offset = None
    _func_opt = None # TODO Применение

    def __init__(self, name=None):
        self.name = name
        self.parent = None

    def read_value(self, stream, obj):
        raise NotImplementedError()

    def parse_stream(self, stream, obj=None):
        if self._func_opt is not None:
            if not self.get_value(self._func_opt, obj):
                return obj

        if obj is None:
            obj = AttrDict()

        if self._offset is not None:
            stream.seek(self.get_value(self._offset, obj))
        val = self.read_value(stream, obj)
        if self.name:  # If name is set, this element must be field id obj, else is parsed into obj
            obj[self.name] = val
        return obj

    def set_parent(self, parent):
        self.parent = parent

    @staticmethod
    def get_value(value, obj):
        if hasattr(value, '__call__'):  # it's lambda or method
            return value(obj)
        return value

    def parse_file(self, path):
        with open(path, "rb") as file:
            return self.parse_stream(file)

    def parse_bytes(self, data):
        return self.parse_stream(io.BytesIO(data))

    def is_bigendian(self):
        if self.byteorder is None and self.parent is not None:
            return self.parent.is_bigendian()
        return self.byteorder == ByteOrder.BE

    def convert(self, func):
        return Converter(self, func)

    def offset(self, func_val):
        self._offset = func_val
        return self

    def optional(self, func_opt):
        self._func_opt = func_opt
        return self


class StructField(Element):
    def __init__(self, name=None, fmt=None):
        """
        :type fmt: str
        """
        super().__init__(name)
        self.fmt = fmt
        self.struct = None

    def read_value(self, stream, obj):
        if not self.struct:
            self.init_struct()

        data = stream.read(self.struct.size)
        if len(data) < self.struct.size:
            raise Exception("Unexcepted end of data")
        unpacked = self.struct.unpack(data)
        return unpacked[0] if len(unpacked) == 1 else unpacked

    def init_struct(self):
        fmt = self.fmt
        if not fmt.startswith('>') and not fmt.startswith('<'):
            if self.is_bigendian():
                fmt = '<' + fmt
            else:
                fmt = '>' + fmt
        self.struct = Struct(fmt)


class BitField:
    def __init__(self, name, size):
        self.name = name
        self.size = size


class BitParser(Element):
    def __init__(self, name=None, fields=None, bitorder=ByteOrder.BE):
        """
        :type fields: BitField[]
        """
        super().__init__(name)
        self.fields = fields
        self.bitorder = bitorder
        self.struct = None
        self.size = None

    def read_value(self, stream, obj):
        if not self.struct:
            self.init_struct()

        if self.name:  # If name is set, this block must be field id obj, else is parsed into obj
            obj = obj[self.name] = AttrDict()

        data = stream.read(self.size)
        s = self.struct.from_buffer_copy(data)
        for f in self.fields:
            v = s.__getattribute__(f.name)
            obj[f.name] = v
        return obj

    def init_struct(self):
        sf = []  # fields for Structure
        for f in self.fields:
            sf.append((f.name, c_ushort, f.size)) # TODO Определение типа

        superclass = self.bitorder == ByteOrder.BE and BigEndianStructure or LittleEndianStructure

        class BitStruct(superclass):
            _pack_ = 1
            _fields_ = sf

        self.struct = BitStruct
        self.size = sizeof(BitStruct)


class ArrayField(Element):
    def __init__(self, name, count, element):
        super().__init__(name)
        self.count = count
        self.element = element
        pass

    def read_value(self, stream, obj):
        count = self.get_value(self.count, obj)
        arr = []
        for _ in range(count):
            e = self.element.parse_stream(stream, obj)
            arr.append(e)
        return arr


class Converter(Element):
    def __init__(self, base, func):
        super().__init__(base.name)
        self.base = base
        self.func = func

    def set_parent(self, parent):
        self.parent = parent
        self.base.parent = self

    def read_value(self, stream, obj):
        return self.func(self.base.read_value(stream, obj))


class BytesField(Element):
    def __init__(self, name=None, size=None):
        super().__init__(name)
        self.size = size

    def read_value(self, stream, obj):
        if self.size:
            return stream.read(self.get_value(self.size, obj))
        return stream.read()


class DataBlock(Element):
    fields = []

    def __init__(self, name=None, fields=None, byteorder=None):
        super().__init__(name)
        if fields:
            self.fields = fields

        for f in self.fields:
            f.set_parent(self)

        if byteorder is not None:
            self.byteorder = byteorder

    def read_value(self, stream, obj=None):
        if self.name:  # If name is set, this block must be field id obj, else is parsed into obj
            d = AttrDict()
            obj[self.name] = d
            obj = d

        for e in self.fields:
            e.parse_stream(stream, obj)
        return obj
