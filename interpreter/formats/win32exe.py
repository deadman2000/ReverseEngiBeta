from structure import DataBlock, StructField

win32exe = DataBlock(fields=[
    StructField('signature', '2s'),
    StructField('lastsize', 'h'),
    StructField('nblocks', 'h'),
    StructField('nreloc', 'h'),
    StructField('hdrsize', 'hhh')
])