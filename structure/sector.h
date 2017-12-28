#ifndef SECTOR_H
#define SECTOR_H

#include "block.h"

class Sector : public Block
{
    friend class Block;
public:
    Sector();
    virtual ~Sector() override;

    virtual int typeID() const override;
    virtual QString typeName() const override;

    virtual void setDataSource(IDataSource *dataSource) override;

    virtual QString toString() const override;

    void append(Block * block);
    void insert(int index, Block * block);
    void replace(int index, Block * block);
    void remove(Block * block);
    const QList<Block *> & childs() const;

    bool load(const QString & fileName);
    bool save(const QString & fileName) const;
    virtual void save(QJsonObject &json) const override;

protected:
    virtual bool updateData() override;
    virtual void readAttr(const QJsonObject &json) override;

    void makeChild(Block * block);

private:
    QList<Block *> _childs;
};

#endif // SECTOR_H
