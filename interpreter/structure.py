import io
from ctypes import c_ushort, LittleEndianStructure, BigEndianStructure, sizeof
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


class StreamWrapper:
    def __init__(self, stream: io.BytesIO, offset=0):
        self.stream = stream
        self.offset = offset  # Offset in global stream

    def read(self, *args, **kwargs):
        return self.stream.read(*args, **kwargs)

    def seek(self, pos):
        return self.stream.seek(pos - self.offset)

    def tell(self):
        return self.offset + self.stream.tell()


class AttrDict(dict):
    def __init__(self, *args, **kwargs):
        super(AttrDict, self).__init__(*args, **kwargs)
        self.__dict__ = self


def get_value(value, obj):
    if hasattr(value, '__call__'):  # it's lambda or method
        return value(obj)
    return value


def reorganize_meta(obj):
    result = {}
    for key, val in obj.items():
        if key.startswith('$'):
            continue

        m = obj['$' + key]
        if m['size'] <= 0:
            continue

        if isinstance(val, AttrDict):
            val = reorganize_meta(val)

        m['value'] = val

        result[key] = m

    return result


class Element(object):
    byteorder = None
    size = None
    offset = None
    func_opt = None
    converters = None
    with_meta = None

    def __init__(self, name=None):
        self.name = name
        self.parent = None

    def read_value(self, stream, obj):
        raise NotImplementedError()

    # Parsing

    def read_stream(self, stream, obj=None):
        if self.func_opt is not None:
            if not get_value(self.func_opt, obj):
                return obj

        if obj is None:
            obj = AttrDict()

        if self.offset is not None:
            offset = get_value(self.offset, obj)
            stream.seek(offset)

        pos = stream.tell()

        if self.size is not None:
            size = get_value(self.size, obj)
            data = stream.read(size)
            stream = StreamWrapper(io.BytesIO(data), pos)

        val = self.read_value(stream, obj)

        size = stream.tell() - pos

        if self.converters is not None:
            for c in self.converters:
                val = c(val)

        if self.name:  # If name is set, this element must be field id obj, else is parsed into obj
            obj[self.name] = val
            if self.is_write_meta():
                meta = {'offset': pos, 'size': size}
                self.write_metainfo(meta, val)
                obj['$' + self.name] = meta

        return obj

    def parse_stream(self, stream, to_meta=False):
        self.with_meta = to_meta
        obj = self.read_stream(StreamWrapper(stream), None)
        if to_meta:
            return reorganize_meta(obj)
        return obj

    def parse_file(self, path, **kwargs):
        with open(path, "rb") as file:
            return self.parse_stream(file, **kwargs)

    def parse_bytes(self, data, **kwargs):
        return self.parse_stream(io.BytesIO(data), **kwargs)

    # Configs

    def set_parent(self, parent):
        self.parent = parent

    def is_bigendian(self):
        if self.byteorder is None and self.parent is not None:
            return self.parent.is_bigendian()
        return self.byteorder == ByteOrder.BE

    # Chaining

    def convert(self, func):
        if self.converters is None:
            self.converters = []
        self.converters.append(func)
        return self

    def set_offset(self, func_val):  # TODO Relative current pos. offset
        self.offset = func_val
        return self

    def optional(self, func_opt):
        self.func_opt = func_opt
        return self

    def set_size(self, size):
        self.size = size
        return self

    # Metainfo

    def write_metainfo(self, meta, obj):
        pass

    def is_write_meta(self):
        if self.with_meta is None and self.parent is not None:
            return self.parent.is_write_meta()
        return self.with_meta


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

    def write_metainfo(self, meta, obj):
        meta['format'] = self.fmt


class ObjectElement(Element):
    def read_value(self, stream, obj):
        if self.name:  # If name is set, this block must be field id obj, else is parsed into obj
            if hasattr(obj, self.name):  # Field already exists. exetend it!
                obj = obj[self.name]
            else:
                d = AttrDict()
                obj[self.name] = d
                obj = d
        self.read_fields(stream, obj)
        return obj

    def read_fields(self, stream, obj):
        raise NotImplementedError()


class BitField:
    def __init__(self, name, size):
        self.name = name
        self.size = size


class BitParser(ObjectElement):
    fields = None

    def __init__(self, name=None, fields=None, bitorder=ByteOrder.BE):
        """
        :type fields: BitField[]
        """
        super().__init__(name)
        if fields:
            self.fields = fields
        self.bitorder = bitorder
        self.struct = None
        self.size = None

    def read_fields(self, stream, obj):
        if not self.struct:
            self.init_struct()

        data = stream.read(self.size)
        s = self.struct.from_buffer_copy(data)
        for f in self.fields:
            v = s.__getattribute__(f.name)
            obj[f.name] = v

    def init_struct(self):
        sf = []  # fields for Structure
        for f in self.fields:
            sf.append((f.name, c_ushort, f.size))  # TODO Определение типа

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
        count = get_value(self.count, obj)
        arr = []
        for _ in range(count):
            if self.element.name is None:
                e = self.element.read_stream(stream)
            else:
                e = self.element.read_stream(stream, obj)
            arr.append(e)
        return arr


class BytesField(Element):
    def __init__(self, name=None, size=None):
        super().__init__(name)
        self.size = size

    def read_value(self, stream, obj):
        if self.size:
            return stream.read(get_value(self.size, obj))
        return stream.read()


class DataBlock(ObjectElement):
    fields = None

    def __init__(self, name=None, fields=None, byteorder=None):
        super().__init__(name)
        if fields:
            self.fields = fields

        for f in self.fields:
            f.set_parent(self)

        if byteorder is not None:
            self.byteorder = byteorder

    def read_fields(self, stream, obj=None):
        for e in self.fields:
            e.read_stream(stream, obj)
