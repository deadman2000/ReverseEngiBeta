#ifndef IDATAINTERPRETER_H
#define IDATAINTERPRETER_H

#include "idatasource.h"

class IDataInterpreter
{
public:
    virtual ~IDataInterpreter();

    virtual QString name() const = 0;

    virtual QString toString(IDataSource * dataSource, int offset) const = 0;
};

#endif // IDATAINTERPRETER_H
