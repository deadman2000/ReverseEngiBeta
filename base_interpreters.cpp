#include "stdafx.h"
#include "base_interpreters.h"
#include "instruments.h"

class SByteInterpreter : public BaseDataInterpreter {
public:
    virtual QString name() const override
    {
        return "Signed byte";
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        std::vector<char> data = dataSource->getData(offset, 1);
        return QString::number(static_cast<signed char>(data[0]));
    }
};

class UByteInterpreter : public BaseDataInterpreter {
public:
    virtual QString name() const override
    {
        return "Unsigned byte";
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        std::vector<char> data = dataSource->getData(offset, 1);
        return QString::number(static_cast<unsigned char>(data[0]));
    }
};

/*bool runtimeIsLittleEndian()
{
    volatile uint16_t i=1;
    return reinterpret_cast<volatile uint8_t*>(&i)[0] == 1;
}*/

void init_base_interpreters()
{
    auto & instr = Instruments::instance();
    instr.registerInterpreter(new SByteInterpreter);
    instr.registerInterpreter(new UByteInterpreter);
}
