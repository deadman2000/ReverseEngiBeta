#include "filedata.h"

FileData::FileData(const char * data, int size)
    : _data(data)
    , _size(size)
{
}

QString FileData::asText()
{
    if (_text.isEmpty())
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
        _text = QString(arr);
        delete[] arr;
    }
    return _text;
}

QString FileData::asHex()
{
    if (_hex.isEmpty())
    {
        QString str("%1 %2 %3 %4 %5 %6 %7 %8  %9 %10 %11 %12 %13 %14 %15 %16");

        for (int i=0; i<_size; i++)
            str = str.arg(_data[i] & 0xFF, 2, 16, QLatin1Char('0'));

        if (_size < 16)
            for (int i =_size; i< 16; i++)
                str = str.arg("  ");

        _hex = str.toUpper();
    }
    return _hex;
}
