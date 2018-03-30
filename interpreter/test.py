from struct import Struct


class BaseElement:
    size = 0
    name = None

    def __init__(self, name):
        self.name = name

    def parse(self, data):
        raise NotImplementedError()


class StringElement(BaseElement):
    encoding = None

    def __init__(self, name, size, enc='ascii'):
        BaseElement.__init__(self, name)
        self.size = size
        self.encoding = enc

    def parse(self, data):
        return data.decode(self.encoding)


class StructElement(BaseElement):
    struct = None

    def __init__(self, name, fmt):
        BaseElement.__init__(self, name)
        self.struct = Struct(fmt)
        self.size = self.struct.size

    def parse(self, data):
        unpacked = self.struct.unpack(data)
        return unpacked[0] if len(unpacked) == 1 else unpacked


class DataBlock:
    elements = [StringElement('signature', 2),
                StructElement('lastsize', 'h'),
                StructElement('nblocks', 'h'),
                StructElement('nreloc', 'h'),
                StructElement('hdrsize', 'hhh')]

    def parse(self, stream):
        result = {}
        for e in self.elements:
            data = stream.read(e.size)
            result[e.name] = e.parse(data)
        return result


block = DataBlock()
res = None
with open("d:/programs/putty.exe", "rb") as file:
    res = block.parse(file)

print(res)