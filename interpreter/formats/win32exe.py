from enum import IntFlag
from structure import DataBlock, StructField, ByteOrder, EEnum, ArrayField
import datetime


# https://en.wikibooks.org/wiki/X86_Disassembly/Windows_Executable_Files
# https://msdn.microsoft.com/ru-ru/library/windows/desktop/ms680547(v=vs.85).aspx
# https://ru.wikipedia.org/wiki/Portable_Executable
# http://www.delorie.com/djgpp/doc/exe/
# http://www.tavi.co.uk/phobos/exeformat.html


def hex_2(val):
    return '%04X' % val


class PeFormat(EEnum):
    PE32 = 0x10b
    PE64 = 0x20b
    ROM = 0x107


class MachineTypes(EEnum):
    UNKNOWN = 0x0
    AM33 = 0x1d3  # Matsushita AM33
    AMD64 = 0x8664  # x64
    ARM64 = 0xaa64  # ARM64 little endian
    ARMNT = 0x1c4  # ARM Thumb-2 little endian
    EBC = 0xebc  # EFI byte code
    I386 = 0x14c  # Intel 386 or later processors and compatible processors
    IA64 = 0x200  # Intel Itanium processor family
    M32R = 0x9041  # Mitsubishi M32R little endian
    MIPS16 = 0x266  # MIPS16
    MIPSFPU = 0x366  # MIPS with FPU
    MIPSFPU16 = 0x466  # MIPS16 with FPU
    POWERPC = 0x1f0  # Power PC little endian
    POWERPCFP = 0x1f1  # Power PC with floating point support
    R4000 = 0x166  # MIPS little endian
    RISCV32 = 0x5032  # RISC-V 32-bit address space
    RISCV64 = 0x5064  # RISC-V 64-bit address space
    RISCV128 = 0x5128  # RISC-V 128-bit address space
    SH3 = 0x1a2  # Hitachi SH3
    SH3DSP = 0x1a3  # Hitachi SH3 DSP
    SH4 = 0x1a6  # Hitachi SH4
    SH5 = 0x1a8  # Hitachi SH5
    ARM = 0x1c0  # ARM little endian
    THUMB = 0x1c2  # Thumb
    ARMV7 = 0x1c4  # ARMv7
    MIPS_R3000 = 0x162  # MIPS R3000
    MIPS_R10000 = 0x168  # MIPS R10000
    WCEMIPSV2 = 0x169  # MIPS little-endian WCE v2
    OLD_ALPHA_AXP = 0x183  # old Alpha AXP
    MOTOROLA_68000 = 0x268  # Motorola 68000 series
    ALPHA_AXP_64 = 0x284  # Alpha AXP 64-bit
    CLR_PURE_MSIL = 0xc0ee  # clr pure MSIL


class Characteristic(IntFlag):
    RELOCS_STRIPPED = 1
    EXECUTABLE_IMAGE = 2
    LINE_NUMS_STRIPPED = 3
    LOCAL_SYMS_STRIPPED = 4
    AGGRESSIVE_WS_TRIM = 5
    LARGE_ADDRESS_AWARE = 6
    RESERVED = 7
    BYTES_REVERSED_LO = 8
    I32BIT_MACHINE = 9
    DEBUG_STRIPPED = 10
    REMOVABLE_RUN_FROM_SWAP = 11
    NET_RUN_FROM_SWAP = 12
    SYSTEM = 13
    DLL = 14
    UP_SYSTEM_ONLY = 15
    BYTES_REVERSED_HI = 16


class DosHeader(DataBlock):
    fields = [
        StructField('signature', '2s'),
        StructField('bytes_in_last_block', 'H'),
        StructField('blocks_in_file', 'H'),
        StructField('num_relocs', 'H'),
        StructField('header_paragraphs', 'H'),
        StructField('min_extra_paragraphs', 'H').convert(hex_2),
        StructField('max_extra_paragraphs', 'H').convert(hex_2),
        StructField('ss', 'H').convert(hex_2),
        StructField('sp', 'H').convert(hex_2),
        StructField('checksum', 'H'),
        StructField('ip', 'H'),
        StructField('cs', 'H'),
        StructField('reloc_table_offset', 'H'),
        StructField('overlay_number', 'H'),
    ]


class ExeReloc(DataBlock):
    fields = [
        StructField('offset', 'H').convert(hex_2),
        StructField('segment', 'H').convert(hex_2),
    ]


class DosExtHeader(DataBlock):
    fields = [
        StructField('reserved1', '4H'),
        StructField('oem_id', 'H'),
        StructField('oem_info', 'H'),
        StructField('reserved2', '10H'),
        StructField('e_lfanew', 'I'),
    ]


class COFFHeader(DataBlock):
    fields = [
        StructField('signature', '4s'),  # Must be PE\0\0
        StructField('machine', 'H').convert(MachineTypes.from_value),
        StructField('sections', 'H'),
        StructField('datetime', 'I').convert(datetime.datetime.fromtimestamp),
        StructField('sym_tbl', 'I'),
        StructField('sym_count', 'I'),
        StructField('size_opt', 'H'),
        StructField('characts', 'H').convert(Characteristic)
    ]


class PEOptHeader(DataBlock):
    fields = [
        StructField('magic', 'H').convert(PeFormat.from_value),
        StructField('major_linker_version', 'B'),
        StructField('minor_linker_version', 'B'),
        StructField('size_of_code', 'I'),
        StructField('size_of_init_data', 'I'),
        StructField('size_of_uninit_data', 'I'),
        StructField('entry_point', 'I'),
        StructField('base_of_code', 'I').convert(hex),
    ]


class DataDirectory(DataBlock):
    fields = [
        StructField('addr', 'I'),
        StructField('size', 'I'),
    ]


class PE64ExtHeader(DataBlock):
    fields = [
        StructField('image_base', 'Q').convert(hex),
        StructField('section_alignment', 'I').convert(hex),
        StructField('file_alignment', 'I').convert(hex),
        StructField('major_os_ver', 'H'),
        StructField('minor_os_ver', 'H'),
        StructField('major_image_ver', 'H'),
        StructField('minor_image_ver', 'H'),
        StructField('major_subs_ver', 'H'),
        StructField('minor_subs_ver', 'H'),
        StructField('win32_version', 'I'),
        StructField('image_size', 'I'),
        StructField('headers_size', 'I'),
        StructField('checksum', 'I'),
        StructField('subsystem', 'H'),
        StructField('dll_chars', 'H'),
        StructField('stacke_reserve_size', 'Q'),
        StructField('stacke_commit_size', 'Q'),
        StructField('heap_reserve_size', 'Q'),
        StructField('heap_commit_size', 'Q'),
        StructField('loader_flags', 'I'),
        StructField('rva_count', 'I'),
        ArrayField('data_dir',
                   count=lambda obj: obj.rva_count,
                   element=DataDirectory()),
    ]


class PE32ExtHeader(DataBlock):
    fields = [
        StructField('base_of_data', 'I').convert(hex),
        StructField('image_base', 'I'),
        StructField('section_alignment', 'I'),
        StructField('file_alignment', 'I'),
        StructField('major_os_ver', 'H'),
        StructField('minor_os_ver', 'H'),
        StructField('major_subs_ver', 'H'),
        StructField('minor_subs_ver', 'H'),
        StructField('win32_version', 'I'),
        StructField('image_size', 'I'),
        StructField('headers_size', 'I'),
        StructField('checksum', 'I'),
        StructField('subsystem', 'H'),
        StructField('dll_chars', 'H'),
        StructField('stacke_reserve_size', 'Q'),
        StructField('stacke_commit_size', 'Q'),
        StructField('heap_reserve_size', 'Q'),
        StructField('heap_commit_size', 'Q'),
        StructField('loader_flags', 'I'),
        StructField('rva_count', 'I'),
        ArrayField('data_dir',
                   count=lambda obj: obj.rva_count,
                   element=DataDirectory()),
    ]


class ImageSection(DataBlock):
    fields = [
        StructField('name', '8s').convert(bytes.decode),
        StructField('misc', 'I'),
        StructField('virtual_addr', 'I').convert(hex),
        StructField('size_of_raw', 'I').convert(hex),
        StructField('pointer_to_raw', 'I').convert(hex),
        StructField('pointer_to_reloc', 'I'),
        StructField('pointer_to_linenum', 'I'),
        StructField('num_reloc', 'H'),
        StructField('num_linemnum', 'H'),
        StructField('characts', 'I'),
    ]

class ExeFormat(DataBlock):
    byteorder = ByteOrder.BE
    fields = [
        DosHeader('dos_header'),

        DataBlock(fields=[  # Windows part
            DosExtHeader('dos_header'),
            COFFHeader('coff_header').set_offset(lambda obj: obj.dos_header.e_lfanew),
            DataBlock(fields=[
                PEOptHeader('pe_opt_header'),
                PE32ExtHeader('pe_opt_header').optional(lambda obj: obj.pe_opt_header.magic == PeFormat.PE32),
                PE64ExtHeader('pe_opt_header').optional(lambda obj: obj.pe_opt_header.magic == PeFormat.PE64),
            ]).set_size(lambda obj: obj.coff_header.size_opt),
            ArrayField('sections',
                       count=lambda obj: obj.coff_header.sections,
                       element=ImageSection()),

        ]).optional(lambda obj: obj.dos_header.reloc_table_offset >= 64),

        ArrayField(
            name='relocs',
            count=lambda obj: obj.dos_header.num_relocs,
            element=ExeReloc()).set_offset(lambda obj: obj.dos_header.reloc_table_offset),
    ]
