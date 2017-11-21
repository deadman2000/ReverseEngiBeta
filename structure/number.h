#ifndef NUMBER_H
#define NUMBER_H

#include "block.h"

namespace structure {

    struct ValueHolder;

    class Number : public Block
    {
        bool _order_big_endian;
        bool _signed;
        ValueHolder * _value;

    public:
        Number();
        virtual ~Number() override;

        void setSize(int size);

        virtual QString toString() const override;

    protected:
        virtual bool updateData() override;
    };
}

#endif // NUMBER_H
