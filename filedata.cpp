#include "filedata.h"

FileData::FileData(std::shared_ptr<FileChunk> chunk)
    : _chunk(chunk)
{
}

QString FileData::asText()
{
    return QString("................");
}

QString FileData::asHex()
{
    /*QString str("%1 %2 %3 %4 %5 %6 %7 %8  %9 %10 %11 %12 %13 %14 %15 %16");
    for (int i=0; i<16; i++)
        str = str.arg(data[i] & 0xFF, 2, 16, QLatin1Char('0'));*/

    return QString("................");
}
