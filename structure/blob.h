#ifndef BLOB_H
#define BLOB_H

#include "block.h"

class Blob : public Block
{
public:
    Blob();

    virtual int typeID() const override;
    virtual QString typeName() const override;
    virtual QString toString() const override;
    virtual bool isValid() const override;

protected:
    virtual bool updateData() override;
    virtual void readAttr(const QJsonObject &json) override;
    virtual void writeAttr(QJsonObject &json) const override;
};

#endif // BLOB_H
