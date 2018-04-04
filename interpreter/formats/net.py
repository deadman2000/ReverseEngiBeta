import ipaddress
from structure import *


class EEnum(Enum):
    @classmethod
    def find_value(cls, value):
        return any(value == item.value for item in cls)

    @classmethod
    def from_value(cls, value):
        return cls.find_value(value) and cls(value) or value


# https://en.wikipedia.org/wiki/Ethernet_frame
class EthernetDataType(EEnum):
    IPv4 = 0x800
    ARP = 0x806
    IPv6 = 0x86DD


class EthernetDataField(Element):
    def parse_stream(self, stream, obj):
        if obj.type == EthernetDataType.IPv4:
            obj['ip'] = ip.parse_stream(stream)
        else:
            obj['other'] = stream.read()


class EthernetPadding(BaseField):
    def __init__(self, name):
        super().__init__(name)

    def parse_field(self, stream, obj):
        return stream.read()


class MacAddressField(BytesField):
    def __init__(self, name):
        super().__init__(name, 6)

    def parse_field(self, stream, obj):
        val = super().parse_field(stream, obj)
        return ':'.join([hex(b)[2:] for b in val])


ethernetii = DataBlock(
    byteorder=ByteOrder.LE,
    fields=[
        MacAddressField('dest'),
        MacAddressField('source'),
        Converter(StructField('type', 'H'), lambda v: EthernetDataType.from_value(v)),
        EthernetDataField(),
        EthernetPadding('padding')
    ])


# http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xml
class IPType(EEnum):
    TCP = 6
    UDP = 17


# https://ru.bmstu.wiki/IPv4_(Internet_Protocol_version_4)
class IPDataField(Element):
    def parse_stream(self, stream, obj):
        size = obj.size - obj.header_size * 4
        data = stream.read(size)

        if obj.protocol == IPType.TCP:
            obj['tcp'] = tcp.parse_bytes(data)
        else:
            obj['other'] = data


ip = DataBlock(
    byteorder=ByteOrder.LE,
    fields=[
        BitParser([
            BitField('version', 4),
            BitField('header_size', 4),
            BitField('dscp', 6),
            BitField('ecn', 2),
        ]),
        StructField('size', 'H'),
        StructField('id', 'H'),
        BitParser([
            BitField('flags', 3),
            BitField('offset', 13),
        ]),
        StructField('ttl', 'B'),
        Converter(StructField('protocol', 'B'), lambda v: IPType.from_value(v)),
        StructField('sum', 'H'),
        Converter(StructField('source', 'I'), lambda v: ipaddress.ip_address(v)),
        Converter(StructField('dest', 'I'), lambda v: ipaddress.ip_address(v)),
        ArrayField(
            name='options',
            count=lambda p: p.header_size - 5,
            element=StructField('options', 'I')),
        IPDataField()
    ])

# https://ru.wikipedia.org/wiki/Transmission_Control_Protocol
tcp = DataBlock(
    byteorder=ByteOrder.LE,
    fields=[
        StructField('source_port', 'H'),
        StructField('dest_port', 'H'),
        StructField('sn', 'I'),
        StructField('ack_sn', 'I'),
        BitParser([
            BitField('header_size', 4),
            BitField('res', 6),
            BitField('flags', 6)
        ]),
        StructField('size', 'H'),
        StructField('check_sum', 'H'),
        StructField('priority', 'H'),
        ArrayField(
            name='options',
            count=lambda p: p.header_size - 5,
            element=StructField('options', 'I')),
        BytesField(name='data')
    ]
)
