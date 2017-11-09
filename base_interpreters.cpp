#include "stdafx.h"

#include "idatainterpreter.h"

class SByteInterpreter : public IDataInterpreter {
    // IDataInterpreter interface
public:
    virtual QString name() const override
    {
        return "Signed byte";
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        auto data = dataSource->getData(offset, 1);
        return QString::number(static_cast<signed char>(data[0]));
    }
};

class UByteInterpreter : public IDataInterpreter {
    // IDataInterpreter interface
public:
    virtual QString name() const override
    {
        return "Unsigned byte";
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        auto data = dataSource->getData(offset, 1);
        return QString::number(data[0]);
    }
};


/*bool runtimeIsLittleEndian()
{
    volatile uint16_t i=1;
    return reinterpret_cast<volatile uint8_t*>(&i)[0] == 1;
}*/
