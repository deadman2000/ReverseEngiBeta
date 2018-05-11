import base64
import datetime
from pprint import pprint

import msgpack

from formats.win32exe import ExeFormat

def encode_datetime(v):
    if isinstance(v, datetime.datetime):
        return v.timestamp()
    return v


#path = "c:/Windows/explorer.exe"
#path = "D:/Dos/GAMES/AGE/intro.exe"
path = "D:/Programs/S&M.exe"
fmt = ExeFormat()
obj = fmt.parse_file(path, to_meta=True, compact_meta=True)  # to_meta=True, compact_meta=True
#pprint(obj['pe_opt_header']['value']['data_dir']['value'])
pprint(obj)

# http://sugendran.github.io/msgpack-visualizer/
data = msgpack.packb(obj, default=encode_datetime, use_bin_type=True)
print('Length:', len(data))
print(base64.b64encode(data))
print(data.hex())
