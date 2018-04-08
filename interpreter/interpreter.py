from winpcapy import WinPcapUtils
from formats.net import EthernetFormat

eth = EthernetFormat()
# filter = ipaddress.IPv4Address('10.10.88.10')


def packet_callback(win_pcap, param, header, pkt_data):
    packet = eth.parse_bytes(pkt_data)
    if hasattr(packet, 'ip') and hasattr(packet.ip, 'tcp') and len(packet.ip.tcp.data) > 0:
        print('%s:%d -> %s:%d = %s' % (
        packet.ip.source, packet.ip.tcp.source_port, packet.ip.dest, packet.ip.tcp.dest_port, packet.ip.tcp.data))


WinPcapUtils.capture_on("*", packet_callback)
