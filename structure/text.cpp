#include "text.h"

namespace structure {

    Text::Text()
        : Block()
        , codec(nullptr)
    {
        //setCodec("KOI8-R");
    }

    void Text::setSize(int size)
    {
        _size = size;
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
}
