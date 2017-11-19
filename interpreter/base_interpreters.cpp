#include "stdafx.h"
#include "base_interpreters.h"
#include "instruments.h"

class NumberInterpreter : public BaseDataInterpreter {
public:
    virtual QString name() const override
    {
        return QString("%1%2 bytes").arg(_signed ? "±" : "+").arg(_size);
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        auto data = dataSource->getData(offset, _size);
        if (data.size() != _size) return QString();

        if (_signed)
            return QString::number(getValue<qulonglong>(data), _base);
        else
            return QString::number(getValue<qlonglong>(data), _base);
    }

    template<typename T>
    T getValue(QByteArray & data) const
    {
        T val = 0;
        if (_byte_order_be)
        {
            for (int i=0; i<data.size(); i++)
            {
                val |= data[i] << (8 * i);
            }
        }
        else
        {
            for (int i=0, n=data.size() - 1; n>=0; i++, n--)
            {
                val |= data[i] << (8 * n);
            }
        }
        return val;
    }

private:
    unsigned char _size; // Количество байт
    unsigned char _base; // Система счисления для перевода в строку
    bool _signed;        // Знаковость. True - Знаковый
    bool _byte_order_be; // Порядок байт. True - Big Endian
};

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
                break;
            }

            data.append(substr);

            if (substr.size() < BLOCK_SIZE)
                break;

            offset += BLOCK_SIZE;
        }

        return QString::fromLocal8Bit(data);
    }
};

class UTFStringZeroTermInterpreter : public BaseDataInterpreter {
public:
    virtual QString name() const override
    {
        return "UTF-8 null-terminated";
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
                break;
            }

            data.append(substr);

            if (substr.size() < BLOCK_SIZE)
                break;

            offset += BLOCK_SIZE;
        }

        return QString::fromUtf8(data);
    }
};

// #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
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
    instr.registerInterpreter(new UTFStringZeroTermInterpreter);
}
