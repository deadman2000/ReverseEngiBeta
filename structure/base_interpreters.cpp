#include "stdafx.h"
#include "base_interpreters.h"
#include "iengine.h"
#include "convert.h"

#define SIGNED   (true)
#define UNSIGNED (false)

#define BIG_ENDIAN (true)
#define LITTLE_ENDIAN (false)

class NumberInterpreter : public BaseDataInterpreter {
public:
    NumberInterpreter(unsigned char size,
                      bool isSigned,
                      bool byte_order_be,
                      unsigned char base = 10)
        : _size(size)
        , _signed(isSigned)
        , _byte_order_be(byte_order_be)
        , _base(base)
    {
    }

    virtual QString name() const override
    {
        return QString("%1%2%3").arg(_byte_order_be ? "<" : ">").arg(_signed ? "" : "u").arg(getTypeName()); // _signed ? "±" : "+"
    }

    virtual QString toString(IDataSource * dataSource, int offset) const override
    {
        auto data = dataSource->getData(offset, _size);
        if (data.size() != _size) return QString();

        if (_signed)
        {
            if (_byte_order_be)
                return QString::number(getValueBE<qlonglong>(data), _base);
            else
                return QString::number(getValueLE<qlonglong>(data), _base);
        }
        else
        {
            if (_byte_order_be)
                return QString::number(getValueBE<qulonglong>(data), _base);
            else
                return QString::number(getValueLE<qulonglong>(data), _base);
        }
    }

protected:
    virtual QString getTypeName() const
    {
        switch (_size)
        {
        case 1: return "byte";
        case 2: return "short";
        case 4: return "int";
        case 8: return "long";
        default: return QString("%1 bytes").arg(_size);
        }
    }

    unsigned char _size;  // Количество байт
    bool _signed;         // Знаковость. True - Знаковый
    bool _byte_order_be;  // Порядок байт. True - Big Endian
    unsigned char _base;  // Система счисления для перевода в строку
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
    engine().registerInterpreter(new NumberInterpreter(8, SIGNED, BIG_ENDIAN));
    engine().registerInterpreter(new SByteInterpreter);
    engine().registerInterpreter(new UByteInterpreter);
    engine().registerInterpreter(new StringZeroTermInterpreter);
    engine().registerInterpreter(new UTFStringZeroTermInterpreter);
}
