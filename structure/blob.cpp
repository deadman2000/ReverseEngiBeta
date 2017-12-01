#include "blob.h"

namespace structure {

    Blob::Blob()
    {
    }

    int Blob::typeID() const
    {
        return 2;
    }

    QString Blob::typeName() const
    {
        return QString("blob");
    }

    QString Blob::toString() const
    {
        return "";
    }

    bool Blob::isValid() const
    {
        return true;
    }

    void Blob::readAttr(const QJsonObject &json)
    {
        Block::readAttr(json);
        setSize(json["size"].toInt());
    }

    void Blob::writeAttr(QJsonObject &json)
    {
        json["size"] = size();
    }

    bool Blob::updateData()
    {
        return true;
    }
}
