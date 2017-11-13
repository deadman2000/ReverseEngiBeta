#ifndef IDATASOURCE_H
#define IDATASOURCE_H

#include <QByteArray>

class IDataSource
{
public:
    virtual ~IDataSource();

    virtual QByteArray getData(int offset, int size) = 0;
};

#endif // IDATASOURCE_H
