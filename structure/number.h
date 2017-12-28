#ifndef NUMBER_H
#define NUMBER_H

#include "block.h"

struct ValueHolder;

class Number : public Block
{
    bool _order_big_endian;
    bool _signed;
    ValueHolder * _value;

public:
    Number();
    virtual ~Number() override;

    virtual int typeID() const override;
    virtual QString typeName() const override;

    virtual QString toString() const override;

protected:
    virtual bool updateData() override;
    void readAttr(const QJsonObject &json) override;
    void writeAttr(QJsonObject &json) const override;
};

#endif // NUMBER_H
