from pprint import pprint

from formats.pcap import PCapFormat

obj = PCapFormat().parse_file('test/dmp4')
pprint(obj)