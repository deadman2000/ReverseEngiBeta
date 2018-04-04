import io
from ctypes import Structure, c_uint8, c_ushort, LittleEndianStructure, BigEndianStructure
from enum import Enum
from struct import Struct

import math


class ByteOrder(Enum):
    BE = 0,
    LE = 1


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)
        self.__dict__ = self


class Element:
    name = None
    parent = None
    byteorder = None

    def parse_stream(self, stream, obj):
        raise NotImplementedError()

    def set_parent(self, parent):
        self.parent = parent

    @staticmethod
    def get_value(value, obj):
        if hasattr(value, '__call__'):  # it's lambda or method
            return value(obj)
        return value

    def is_bigendian(self):
        if self.byteorder is None and self.parent is not None:
            return self.parent.is_bigendian()
        return self.byteorder == ByteOrder.BE


class BaseField(Element):
    def __init__(self, name):
        self.name = name

    def parse_stream(self, stream, obj):
        obj[self.name] = self.parse_field(stream, obj)

    def parse_field(self, stream, obj):
        raise NotImplementedError()


class StructField(BaseField):
    def __init__(self, name=None, fmt=None):
        """

        :type fmt: str
        """
        super().__init__(name)
        self.fmt = fmt
        self.struct = None

    def parse_field(self, stream, obj):
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
    def __init__(self, fields, bitorder=ByteOrder.BE):
        """
        :type fields: BitField[]
        """
        bitcount = 0
        for f in fields:
            bitcount += f.size
        self.fields = fields
        self.bytecount = int(math.ceil(bitcount / 8))
        self.struct = None
        self.bitorder = bitorder

    def parse_stream(self, stream, obj):
        if not self.struct:
            self.init_struct()

        data = stream.read(self.bytecount)
        s = self.struct.from_buffer_copy(data)
        for f in self.fields:
            v = s.__getattribute__(f.name)
            obj[f.name] = v

    def init_struct(self):
        sf = []  # fields for Structure
        for f in self.fields:
            sf.append((f.name, c_ushort, f.size))

        superclass = self.bitorder == ByteOrder.BE and BigEndianStructure or LittleEndianStructure

        class BitStruct(superclass):
            _pack_ = 1
            _fields_ = sf

        self.struct = BitStruct


class DataField(BaseField):
    def __init__(self, name, size=None):
        super().__init__(name)
        self.size = size

    def parse_field(self, stream, obj):
        size = self.get_value(self.size, obj)
        return stream.read(size)


class ArrayField(BaseField):
    count = None
    element = None

    def __init__(self, name, count, element):
        super().__init__(name)
        self.count = count
        self.element = element
        pass

    def parse_field(self, stream, obj):
        count = self.get_value(self.count, obj)
        arr = []
        for _ in range(count):
            e = self.element.parse_stream(stream, obj)
            arr.append(e)
        return arr


class Converter(BaseField):
    def __init__(self, element, func):
        super().__init__(element.name)
        self.element = element
        self.func = func

    def set_parent(self, parent):
        self.parent = parent
        self.element.parent = parent

    def parse_field(self, stream, obj):
        return self.func(self.element.parse_field(stream, obj))


class BytesField(BaseField):
    def __init__(self, name=None, size=None):
        super().__init__(name)
        self.size = size

    def parse_field(self, stream, obj):
        if self.size:
            return stream.read(self.get_value(self.size, obj))
        return stream.read()


class DataBlock(Element):
    fields = None
    byteorder = None

    def __init__(self, fields=None, byteorder=None):
        self.fields = fields or []
        for f in fields:
            f.set_parent(self)
        self.byteorder = byteorder

    def parse_stream(self, stream, obj=None):
        obj = obj or AttrDict()
        for e in self.fields:
            e.parse_stream(stream, obj)
        return obj

    def parse_file(self, path):
        with open(path, "rb") as file:
            return self.parse_stream(file)

    def parse_bytes(self, data):
        return self.parse_stream(io.BytesIO(data))

    def is_bigendian(self):
        if self.byteorder is None and self.parent is not None:
            return self.parent.is_bigendian()
        return self.byteorder == ByteOrder.BE
