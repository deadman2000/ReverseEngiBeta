#ifndef CONVERT_H
#define CONVERT_H

#include <QByteArray>

template<typename T>
T getValueBE(QByteArray & data)
{
    T val = 0;
    for (int i=0; i<data.size(); i++)
    {
        val |= ((uchar)data[i]) << (i << 3);
    }
    return val;
}

template<typename T>
T getValueLE(QByteArray & data)
{
    T val = 0;
    for (int i=0; i<data.size(); i++)
    {
        val = (val << 8) | data[i];
    }
    return val;
}

#endif // CONVERT_H
