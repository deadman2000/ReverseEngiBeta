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
        auto data = dataSource->getData(offset, 1);
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
        auto data = dataSource->getData(offset, 1);
        return QString::number(static_cast<unsigned char>(data[0]));
    }
};

class StringZeroTermInterpreter : public BaseDataInterpreter {
public:
    virtual QString name() const override
    {
        return "String null-terminated";
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        const int BLOCK_COUNT = 16;
        const int BLOCK_SIZE = 16;

        QByteArray data;
        for (int i=0; i<BLOCK_COUNT; i++)
        {
            auto substr = dataSource->getData(offset, BLOCK_SIZE);

            int n = substr.indexOf('\0');

            if (n != -1)
            {
                substr.truncate(n + 1);
                data.append(substr);
                return QString(data);
            }

            data.append(substr);

            if (substr.size() < BLOCK_SIZE)
                break;

            offset += BLOCK_SIZE;
        }

        return "";
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
    instr.registerInterpreter(new StringZeroTermInterpreter);
}
