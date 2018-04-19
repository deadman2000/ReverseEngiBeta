#ifndef CONVERT_H
#define CONVERT_H

#include <QByteArray>

qlonglong getValueBESign(QByteArray & data)
{
    qlonglong val = 0;
    for (int i=0; i<data.size(); i++)
        val |= ((uchar)data[i]) << (i << 3);

    if (data[data.size() - 1] & 80)
        val = -val;

    return val;
}

qulonglong getValueBEUnsign(QByteArray & data)
{
    qulonglong val = 0;
    for (int i=0; i<data.size(); i++)
        val |= ((uchar)data[i]) << (i << 3);
    return val;
}

qlonglong getValueLESign(QByteArray & data)
{
    qlonglong val = 0;
    for (int i=0; i<data.size(); i++)
        val = (val << 8) | data[i];

    if (data[0] & 80)
        val = -val;

    return val;
}

qlonglong getValueLEUnsign(QByteArray & data)
{
    qlonglong val = 0;
    for (int i=0; i<data.size(); i++)
        val = (val << 8) | data[i];
    return val;
}

#endif // CONVERT_H
