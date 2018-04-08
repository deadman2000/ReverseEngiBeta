import ipaddress
from structure import *


# https://ru.wikipedia.org/wiki/Transmission_Control_Protocol
class TCPFormat(DataBlock):
    byteorder = ByteOrder.LE
    fields = [
        StructField('source_port', 'H'),
        StructField('dest_port', 'H'),
        StructField('sn', 'I'),
        StructField('ack_sn', 'I'),
        BitParser(fields=[
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


# http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xml
class IPProtocol(EEnum):
    TCP = 6
    UDP = 17


# https://ru.bmstu.wiki/IPv4_(Internet_Protocol_version_4)
class IPDataField(Element):
    def __init__(self):
        super().__init__()
        self.tcp = TCPFormat()

    def read_value(self, stream, obj):
        size = obj.size - obj.header_size * 4
        data = stream.read(size)
        if obj.protocol == IPProtocol.TCP:
            obj['tcp'] = self.tcp.parse_bytes(data)
        else:
            obj['other'] = data


class IPAddr(StructField):
    def __init__(self, name):
        super().__init__(name, 'I')
        self.convert(ipaddress.ip_address)


class IPFormat(DataBlock):
    byteorder = ByteOrder.LE
    fields = [
        BitParser(fields=[
            BitField('version', 4),
            BitField('header_size', 4),
            BitField('dscp', 6),
            BitField('ecn', 2),
        ]),
        StructField('size', 'H'),
        StructField('id', 'H'),
        BitParser(fields=[
            BitField('flags', 3),
            BitField('offset', 13),
        ]),
        StructField('ttl', 'B'),
        StructField('protocol', 'B').convert(IPProtocol.from_value),
        StructField('sum', 'H'),
        IPAddr('source'),
        IPAddr('dest'),
        ArrayField(
            name='options',
            count=lambda p: p.header_size - 5,
            element=StructField('options', 'I')),
        IPDataField()
    ]


# https://en.wikipedia.org/wiki/Ethernet_frame
class EthernetDataType(EEnum):
    IPv4 = 0x800
    ARP = 0x806
    IPv6 = 0x86DD


class EthernetDataField(DataBlock):
    fields=[
        IPFormat('ip').optional(lambda obj: obj.type == EthernetDataType.IPv4),
        BytesField('other').optional(lambda obj: obj.type != EthernetDataType.IPv4),
    ]

class MacAddressField(BytesField):
    def __init__(self, name):
        super().__init__(name, 6)
        self.convert(lambda v: ':'.join([hex(b)[2:] for b in v]))


class EthernetFormat(DataBlock):
    byteorder = ByteOrder.LE
    fields = [
        MacAddressField('dest'),
        MacAddressField('source'),
        StructField('type', 'H').convert(EthernetDataType.from_value),
        EthernetDataField(),
        BytesField('padding')
    ]
