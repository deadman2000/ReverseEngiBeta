from structure import DataBlock, StructField, ByteOrder, BytesField, EEnum
import datetime


# https://en.wikibooks.org/wiki/X86_Disassembly/Windows_Executable_Files
# https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms680547(v=vs.85).aspx
# https://ru.wikipedia.org/wiki/Portable_Executable
class DosHeader(DataBlock):
    fields = [
        StructField('signature', '2s'),
        StructField('lastsize', 'H'),
        StructField('nblocks', 'H'),
        StructField('nreloc', 'H'),
        StructField('hdrsize', 'H'),
        StructField('minalloc', 'H'),
        StructField('maxalloc', 'H'),
        StructField('ss', 'H'),
        StructField('sp', 'H'),
        StructField('checksum', 'H'),
        StructField('ip', 'H'),
        StructField('cs', 'H'),
        StructField('relocpos', 'H'),
        StructField('noverlay', 'H'),
        StructField('reserved1', '4H'),
        StructField('oem_id', 'H'),
        StructField('oem_info', 'H'),
        StructField('reserved2', '10H'),
        StructField('e_lfanew', 'I'),
    ]


class COFFHeader(DataBlock):
    fields = [
        StructField('signature', '4s'),
        StructField('machine', 'H').convert(hex),
        StructField('sections', 'H'),
        StructField('datetime', 'I').convert(datetime.datetime.fromtimestamp),
        StructField('sym_tbl', 'I'),
        StructField('sym_count', 'I'),
        StructField('size_opt', 'H'),
        StructField('characts', 'H').convert(bin)
    ]


class PeFormat(EEnum):
    PE32 = 0x10b
    PE32P = 0x20b


class COFFFields(DataBlock):
    fields = [
        StructField('magic', 'H').convert(PeFormat.from_value),
        StructField('major_linker_version', 'B'),
        StructField('minor_linker_version', 'B'),
        StructField('size_of_code', 'I'),
        StructField('size_of_init_data', 'I'),
        StructField('size_of_uninit_data', 'I'),
        StructField('entry_point', 'I'),
        StructField('base_of_code', 'I'),
        StructField('base_of_data', 'I').optional(lambda obj: obj.magic == PeFormat.PE32),
    ]


class OptHeader(DataBlock):
    fields = [
    ]


class WinExeFormat(DataBlock):
    byteorder = ByteOrder.BE
    fields = [
        DosHeader('dos_header'),
        COFFHeader('coff_header').offset(lambda obj: obj.dos_header.e_lfanew),
        COFFFields('opt_header')
    ]
