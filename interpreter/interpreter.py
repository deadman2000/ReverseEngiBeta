import structformat
from structure import DataBlock

#block = structformat.read_str_file('win_exe.str')

block = DataBlock()
print(block.parse_file("d:/programs/miniroute.exe"))
