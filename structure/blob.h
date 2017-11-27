#ifndef BLOB_H
#define BLOB_H

#include "block.h"

namespace structure {

    class Blob : public Block
    {
    public:
        Blob();

        virtual int typeID() const override;
        virtual QString toString() const override;
        virtual bool isValid() const override;

    protected:
        virtual bool updateData() override;
        virtual void readAttr(const QJsonObject &json) override;
        virtual void writeAttr(QJsonObject &json) override;
    };

}

#endif // BLOB_H
