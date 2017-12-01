#ifndef TEXT_H
#define TEXT_H

#include <QTextCodec>
#include "block.h"

namespace structure {

    class Text : public Block
    {
    public:
        Text();

        virtual int typeID() const override;
        virtual QString typeName() const override;

        QString toString() const override;

        void setCodec(const QByteArray &name);

    protected:
        bool updateData() override;
        virtual void readAttr(const QJsonObject &json) override;
        virtual void writeAttr(QJsonObject &json) override;

    private:
        QByteArray _terminator;
        QTextCodec * codec;
    };
}

#endif // TEXT_H
