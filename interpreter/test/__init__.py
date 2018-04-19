import pprint
import unittest
from ipaddress import IPv4Address
from formats.net import *


ETHERNET_DATA = '0007b4004d015404a660dc5e08004500006c2dd34000800600000a0a4e0771939aee7dbc24f5415c87ede7cc36f45018010264f1000013426974546f7272656e742070726f746f636f6c00000000001800054fbdafe756fcb629ce19178de30a26af5b934ca62d7142343033302d4f72725a55767452446a4762'


class TestStringMethods(unittest.TestCase):
    maxDiff = None

    def test_bits_le(self):
        obj = BitParser(fields=[
            BitField('a', 1),
            BitField('b', 1),
            BitField('c', 1),
            BitField('d', 1),
            BitField('e', 1),
            BitField('f', 1),
            BitField('g', 1),
            BitField('h', 1),

            BitField('i', 1),
            BitField('j', 1),
            BitField('k', 1),
            BitField('l', 1),
            BitField('m', 1),
            BitField('n', 1),
            BitField('o', 1),
            BitField('p', 1),
        ]).parse_bytes(b'\x64\xc7')
        self.assertDictEqual(obj, {'a': 0, 'b': 1, 'c': 1, 'd': 0, 'e': 0, 'f': 1, 'g': 0, 'h': 0, 'i': 1, 'j': 1, 'k': 0, 'l': 0, 'm': 0, 'n': 1, 'o': 1, 'p': 1})

    def test_bits_be(self):
        obj = BitParser(fields=[
            BitField('a', 1),
            BitField('b', 1),
            BitField('c', 1),
            BitField('d', 1),
            BitField('e', 1),
            BitField('f', 1),
            BitField('g', 1),
            BitField('h', 1),

            BitField('i', 1),
            BitField('j', 1),
            BitField('k', 1),
            BitField('l', 1),
            BitField('m', 1),
            BitField('n', 1),
            BitField('o', 1),
            BitField('p', 1),
        ]).set_bigendian().parse_bytes(b'\x64\xc7')
        self.assertDictEqual(obj, {'a': 0, 'b': 0, 'c': 1, 'd': 0, 'e': 0, 'f': 1, 'g': 1, 'h': 0, 'i': 1, 'j': 1, 'k': 1, 'l': 0, 'm': 0, 'n': 0, 'o': 1, 'p': 1})

    def test_optional(self):
        obj = DataBlock(fields=[
            NumberField('a', 1).optional(True),
            NumberField('b', 1).optional(False),
            NumberField('c', 1),
            NumberField('d', 1).optional(lambda o: o.c == 2),
            NumberField('e', 1).optional(lambda o: o.c == 48)
        ]).parse_bytes(b'\x01\x02\x03')
        self.assertDictEqual(obj, {'a': 1, 'c': 2, 'd': 3})

    def test_eth_ip_tcp(self):
        ethernet = EthernetFormat()
        data = bytes.fromhex(ETHERNET_DATA)
        result = ethernet.parse_bytes(data)
        example = {
            'dest': '0:7:b4:0:4d:1',
            'ip': {'dest': IPv4Address('113.147.154.238'),
                   'dscp': 0,
                   'ecn': 0,
                   'flags': 2,
                   'header_size': 5,
                   'id': 11731,
                   'offset': 0,
                   'options': '',
                   'protocol': IPProtocol.TCP,
                   'size': 108,
                   'source': IPv4Address('10.10.78.7'),
                   'sum': 0,
                   'tcp': {'ack_sn': 3888920308,
                           'checksum': 25841,
                           'data': b'\x13BitTorrent protocol\x00\x00\x00\x00'
                                   b'\x00\x18\x00\x05O\xbd\xaf\xe7V\xfc\xb6)'
                                   b'\xce\x19\x17\x8d\xe3\n&\xaf[\x93L\xa6-qB4030-OrrZUvtR'
                                   b'DjGb',
                           'dest_port': 9461,
                           'flags': 24,
                           'header_size': 5,
                           'options': '',
                           'res': 0,
                           'size': 258,
                           'sn': 1096583149,
                           'src_port': 32188,
                           'urg': 0},
                   'ttl': 128,
                   'version': 4},
            'padding': b'',
            'source': '54:4:a6:60:dc:5e',
            'type': EthernetDataType.IPv4
        }
        #pprint.pprint(result)
        self.assertDictEqual(result, example)


if __name__ == '__main__':
    unittest.main()
