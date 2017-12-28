#include "text.h"

Text::Text()
    : Block()
    , codec(nullptr)
{
    //setCodec("KOI8-R");
}

int Text::typeID() const
{
    return 1;
}

QString Text::typeName() const
{
    return "text";
}

QString Text::toString() const
{
    if (_isValid)
    {
        QByteArray data = _dataSource->getData(_offset, _size);
        if (codec)
            return codec->toUnicode(data);
        else
            return QString::fromUtf8(data);
    }
    return "";
}

void Text::setCodec(const QByteArray &name)
{
    codec = QTextCodec::codecForName(name);
}

bool Text::updateData()
{
    return true;
}

void Text::readAttr(const QJsonObject &json)
{
    setSize(json["size"].toInt());
}

void Text::writeAttr(QJsonObject &json) const
{
    json["size"] = _size;
}
