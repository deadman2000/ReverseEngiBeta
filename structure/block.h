#ifndef BLOCK_H
#define BLOCK_H

#include "idatasource.h"
#include "addressrange.h"

class Sector;

class Block
{
protected:
    IDataSource * _dataSource;
    Sector * _parent;
    int _offset; // Позиция в файле. Если < 0, позиция не известна
    int _size;
    bool _isValid;

    QString _name;
    QString _typeName;
    QString _description;
    QString _valueStr;
    AddressRange _range;

public:
    Block();
    virtual ~Block();

    virtual void setDataSource(IDataSource * dataSource);

    void setParent(Sector * parent);
    Sector * parent() const;

    virtual QString title() const;

    virtual const QString & typeName() const;
    void setTypeName(const QString & typeName);

    void setOffset(int offset);
    int offset() const;

    int size() const;
    void setSize(int value);

    virtual bool isValid() const;

    const QString & name() const;
    void setName(const QString & name);

    void setDescription(const QString & description);

    void setValueStr(const QString & valueStr);
    virtual QString valueStr() const;

    void remove();

    Sector * as_sector();

    AddressRange * range();
};

#endif // BLOCK_H
