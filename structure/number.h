#ifndef NUMBER_H
#define NUMBER_H

#include "block.h"

namespace structure {

    class Number : public Block
    {
        int _size;
        bool _order_big_endian;
        bool _signed;
        qlonglong _value;

    public:
        Number();

        virtual QString toString() const override;
        virtual bool isValid() const override;
        virtual int getSize() const override;

    protected:
        virtual void updateData() override;
    };
}

#endif // NUMBER_H
