import unittest
from ipaddress import IPv4Address
from formats.net import *


class TestStringMethods(unittest.TestCase):
    maxDiff = None

    def test_bits(self):
        bp = BitParser(fields=[
            BitField('a', 1),
            BitField('b', 1),
            BitField('c', 1),
            BitField('d', 1),
            BitField('e', 1),
            BitField('f', 1),
            BitField('g', 1),
            BitField('h', 1),
        ])
        # bp.parse_bytes(b'\x64')

    def test_optional(self):
        block = DataBlock(fields=[
            StructField('a', 'B').optional(True),
            StructField('b', 'B').optional(False),
            StructField('c', 'B'),
            StructField('d', 'B').optional(lambda o: o.c == 2),
            StructField('e', 'B').optional(lambda o: o.c == 48)
        ])
        obj = block.parse_bytes(b'\x01\x02\x03')
        self.assertDictEqual(obj, {'a': 1, 'c': 2, 'd': 3})

    def test_eth_ip_tcp(self):
        ethernet = EthernetFormat()
        data = bytes.fromhex(
            '0007b4004d015404a660dc5e08004500006c2dd34000800600000a0a4e0771939aee7dbc24f5415c87ede7cc36f45018010264f1000013426974546f7272656e742070726f746f636f6c00000000001800054fbdafe756fcb629ce19178de30a26af5b934ca62d7142343033302d4f72725a55767452446a4762')
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
                   'options': [],
                   'protocol': IPProtocol.TCP,
                   'size': 108,
                   'source': IPv4Address('10.10.78.7'),
                   'sum': 0,
                   'tcp': {'ack_sn': 3888920308,
                           'check_sum': 25841,
                           'data': b'\x13BitTorrent protocol\x00\x00\x00\x00'
                                   b'\x00\x18\x00\x05O\xbd\xaf\xe7V\xfc\xb6)'
                                   b'\xce\x19\x17\x8d\xe3\n&\xaf[\x93L\xa6-qB4030-OrrZUvtR'
                                   b'DjGb',
                           'dest_port': 9461,
                           'flags': 24,
                           'header_size': 5,
                           'options': [],
                           'priority': 0,
                           'res': 0,
                           'size': 258,
                           'sn': 1096583149,
                           'source_port': 32188},
                   'ttl': 128,
                   'version': 4},
            'padding': b'',
            'source': '54:4:a6:60:dc:5e',
            'type': EthernetDataType.IPv4
        }
        self.assertDictEqual(result, example)


if __name__ == '__main__':
    unittest.main()
