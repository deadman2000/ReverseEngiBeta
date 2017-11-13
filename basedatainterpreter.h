#ifndef IDATAINTERPRETER_H
#define IDATAINTERPRETER_H

#include "idatasource.h"
#include <QString>

class BaseDataInterpreter
{
public:
    BaseDataInterpreter();

    virtual ~BaseDataInterpreter();

    virtual QString name() const = 0;

    virtual QString toString(IDataSource * dataSource, int offset) const = 0;
};

#endif // IDATAINTERPRETER_H
