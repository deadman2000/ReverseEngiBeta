#include "filedata.h"

FileData::FileData(const char * data, int size)
    : _data(data)
    , _size(size)
{
}

QString FileData::asText()
{
    char * arr = new char[_size + 1];
    for (int i=0; i<_size; i++)
    {
        if (_data[i] < 32)
            arr[i] = '.';
        else
            arr[i] = _data[i];
    }
    arr[_size] = 0;
    QString text = QString(arr);
    delete[] arr;
    return text;
}

inline char toHex(char number)
{
    Q_ASSERT(number >= 0 && number < 0x10);
    if (number < 0xA)
        return 0x30 + number;
    else
        return 0x37 + number;
}

QString FileData::asHex()
{
    QByteArray symbols(_size * 2 + _size - 1, ' ');

    for (int i=0; i<_size; i++)
    {
        symbols[i * 3] = toHex((_data[i] >> 4) & 0xf);
        symbols[i * 3 + 1] = toHex(_data[i] & 0xf);
    }

    return QString(symbols);
}
