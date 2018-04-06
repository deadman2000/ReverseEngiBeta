from pprint import pprint

from formats.win32exe import WinExeFormat


path = "c:/Windows/System32/calc.exe"
pprint(WinExeFormat().parse_file(path))
