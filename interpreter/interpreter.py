import ipaddress

from formats.net import ethernetii
from winpcapy import WinPcapUtils


filter = ipaddress.IPv4Address('10.10.88.10')

def packet_callback(win_pcap, param, header, pkt_data):
    #try:
    eth = ethernetii.parse_bytes(pkt_data)
    if hasattr(eth, 'ip'):
        if hasattr(eth.ip, 'tcp'):
            print(eth.ip.tcp)
    #except Exception as e:
    #    print(e)

WinPcapUtils.capture_on("*", packet_callback)


#from formats.win32exe import win32exe
#print(win32exe.parse_file("d:/programs/miniroute.exe"))
