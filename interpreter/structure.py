from struct import Struct


class Element:
    parent = None
    name = None


class BaseField(Element):
    def __init__(self, name):
        self.name = name

    def parse(self, stream):
        raise NotImplementedError()


class StructField(BaseField):
    struct = None

    def __init__(self, name, fmt):
        BaseField.__init__(self, name)
        self.struct = Struct(fmt)

    def parse(self, stream):
        unpacked = self.struct.unpack(stream.read(self.struct.size))
        return unpacked[0] if len(unpacked) == 1 else unpacked


class DataBlock(Element):
    elements = [StructField('signature', '2s'),
                StructField('lastsize', 'h'),
                StructField('nblocks', 'h'),
                StructField('nreloc', 'h'),
                StructField('hdrsize', 'hhh')]

    def parse_stream(self, stream):
        result = {}
        for e in self.elements:
            result[e.name] = e.parse(stream)
        return result

    def parse_file(self, path):
        with open(path, "rb") as file:
            return self.parse_stream(file)

    def add_child(self, element):
        self.elements.append(element)
        element.parent = self
