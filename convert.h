#ifndef CONVERT_H
#define CONVERT_H

#include <QByteArray>

template<typename T>
T getValueBE(QByteArray & data)
{
    T val = 0;
    for (int i=0; i<data.size(); i++)
    {
        val |= data[i] << (8 * i);
    }
    return val;
}

template<typename T>
T getValueLE(QByteArray & data)
{
    T val = 0;
    for (int i=0, n=data.size() - 1; n>=0; i++, n--)
    {
        val |= data[i] << (8 * n);
    }
    return val;
}

#endif // CONVERT_H
