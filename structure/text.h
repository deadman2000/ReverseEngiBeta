#ifndef TEXT_H
#define TEXT_H

#include <QTextCodec>
#include "block.h"

namespace structure {

    class Text : public Block
    {
    public:
        Text();

        void setSize(int size);

        QString toString() const override;

        void setCodec(const QByteArray &name);

    protected:
        bool updateData() override;

    private:
        QByteArray _terminator;
        QTextCodec * codec;
    };
}

#endif // TEXT_H
