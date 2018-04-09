import datetime
from pprint import pprint
from formats.win32exe import ExeFormat

def encode_datetime(v):
    if isinstance(v, datetime.datetime):
        return v.timestamp()
    return v


path = "c:/Windows/explorer.exe"
# path = "D:/Dos/GAMES/AGE/intro.exe"
fmt = ExeFormat()
#fmt.with_meta = True
obj = fmt.parse_file(path, to_meta=True)  # to_meta=True
pprint(obj['pe_opt_header']['value']['data_dir']['value'])

# http://sugendran.github.io/msgpack-visualizer/
#data = msgpack.packb(obj, default=encode_datetime, use_bin_type=True)
#print(base64.b64encode(data))
#print(data.hex())
