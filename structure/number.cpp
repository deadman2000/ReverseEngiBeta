#include "number.h"

#include "convert.h"

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

int Number::typeID() const
{
    return 0;
}

QString ctype(int size)
{
    switch (size)
    {
    case 1: return "byte";
    case 2: return "short";
    case 4: return "int";
    case 8: return "long";
    default: return QString("%1 bytes").arg(size);
    }
}

QString Number::typeName() const
{
    return QString("%1%2").arg(_signed ? "" : "u").arg(ctype(_size));
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

void Number::readAttr(const QJsonObject &json)
{
    _order_big_endian = json["bigendian"].toBool();
    _signed = json["signed"].toBool();
    setSize(json["size"].toInt());
}

void Number::writeAttr(QJsonObject &json) const
{
    json["bigendian"] = _order_big_endian;
    json["signed"] = _signed;
    json["size"] = _size;
}
