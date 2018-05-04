import ipaddress
from structure import *


# https://ru.wikipedia.org/wiki/Transmission_Control_Protocol
class TCPFormat(DataBlock):
    byteorder = ByteOrder.LE
    fields = [
        NumberField('src_port', 2).display('Source Port'),
        NumberField('dst_port', 2).display('Destination Port'),
        NumberField('sn', 4).display('Sequence Number'),
        NumberField('ack_sn', 4).display('Acknowledgment Number'),
        BitParser(fields=[
            BitField('header_size', 4).display('Header Size'),
            BitField('res', 3).display('Reserved'),
            BitField('flags', 9).display('Flags')
        ]),
        NumberField('size', 2),
        NumberField('checksum', 2),
        NumberField('urg', 2).display('Urgen Pointer'),
        BytesField('options', lambda p: (p.header_size - 5) * 4).convert(bytes.hex),
        BytesField(name='data')
    ]


# https://ru.wikipedia.org/wiki/UDP
class UDPFormat(DataBlock):
    byteorder = ByteOrder.LE
    fields = [
        NumberField('src_port', 2).display('Source Port'),
        NumberField('dst_port', 2).display('Destination Port'),
        NumberField('size', 2),
        NumberField('checksum', 2),
        BytesField(name='data').validation(lambda v, obj: len(v) == obj.size - 8)
    ]


# http://www.iana.org/assignments/protocol-numbers/protocol-numbers.xml
class IPProtocol(EEnum):
    ICMP = 1
    TCP = 6
    UDP = 17


# https://ru.bmstu.wiki/IPv4_(Internet_Protocol_version_4)
class IPDataField(Element):
    def __init__(self):
        super().__init__()
        self.tcp = TCPFormat()
        self.udp = UDPFormat()

    def read_value(self, stream, obj):
        size = obj.size - obj.header_size * 4
        data = stream.read(size)
        if obj.protocol == IPProtocol.TCP:
            obj['tcp'] = self.tcp.parse_bytes(data)
        elif obj.protocol == IPProtocol.UDP:
            obj['udp'] = self.udp.parse_bytes(data)
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
        BytesField('options', lambda p: (p.header_size - 5) * 4).convert(bytes.hex),
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
