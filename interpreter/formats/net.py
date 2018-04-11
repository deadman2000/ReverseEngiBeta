import ipaddress
from structure import *


# https://ru.wikipedia.org/wiki/Transmission_Control_Protocol
class TCPFormat(DataBlock):
    byteorder = ByteOrder.LE
    fields = [
        NumberField('source_port', 2),
        NumberField('dest_port', 2),
        NumberField('sn', 4),
        NumberField('ack_sn', 4),
        BitParser(fields=[
            BitField('header_size', 4),
            BitField('res', 6),
            BitField('flags', 6)
        ]),
        NumberField('size', 2),
        NumberField('check_sum', 2),
        NumberField('priority', 2),
        ArrayField(
            name='options',
            count=lambda p: p.header_size - 5,
            element=NumberField(bytes_count=4)),
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


class IPAddr(NumberField):
    def __init__(self, name):
        super().__init__(name, 4)
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
        NumberField('size', 2),
        NumberField('id', 2),
        BitParser(fields=[
            BitField('flags', 3),
            BitField('offset', 13),
        ]),
        NumberField('ttl', 1),
        NumberField('protocol', 1).convert(IPProtocol.from_value),
        NumberField('sum', 2),
        IPAddr('source'),
        IPAddr('dest'),
        ArrayField(
            name='options',
            count=lambda p: p.header_size - 5,
            element=NumberField(bytes_count=4)),
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
        NumberField('type', 2).convert(EthernetDataType.from_value),
        EthernetDataField(),
        BytesField('padding')
    ]
