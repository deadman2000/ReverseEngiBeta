#include "number.h"

#include "convert.h"

namespace structure {
    struct ValueHolder {
        virtual QString toString() = 0;
        virtual ~ValueHolder(){}
    };

    template<typename T>
    struct CustomValueHolder : ValueHolder
    {
        T value;

        CustomValueHolder(QByteArray & data, bool big_endian)
        {
            if (big_endian)
                value = getValueBE<T>(data);
            else
                value = getValueLE<T>(data);
        }

        QString toString() override {
            return QString::number(value);
        }
    };

    Number::Number()
        : _order_big_endian(true)
        , _signed(false)
        , _value(nullptr)
    {
    }

    Number::~Number()
    {
        if (_value)
            delete _value;
    }

    void Number::setSize(int size)
    {
        Q_ASSERT(size > 0);
        _size = size;
    }

    QString Number::toString() const
    {
        if (_value)
            return _value->toString();
        return "";
    }

    bool Number::updateData()
    {
        QByteArray data = _dataSource->getData(_offset, _size);
        if (data.size() != _size)
            return false;

        if (_signed)
        {
            switch (_size) {
            case 1:
                _value = new CustomValueHolder<qint8>(data, _order_big_endian);
                break;
            case 2:
                _value = new CustomValueHolder<qint16>(data, _order_big_endian);
                break;
            case 3:
            case 4:
                _value = new CustomValueHolder<qint32>(data, _order_big_endian);
                break;
            default:
                _value = new CustomValueHolder<qlonglong>(data, _order_big_endian);
                break;
            }
        }
        else
        {
            switch (_size) {
            case 1:
                _value = new CustomValueHolder<quint8>(data, _order_big_endian);
                break;
            case 2:
                _value = new CustomValueHolder<quint16>(data, _order_big_endian);
                break;
            case 3:
            case 4:
                _value = new CustomValueHolder<quint32>(data, _order_big_endian);
                break;
            default:
                _value = new CustomValueHolder<qulonglong>(data, _order_big_endian);
                break;
            }
        }

        return true;
    }

}
