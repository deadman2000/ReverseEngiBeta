#ifndef SECTOR_H
#define SECTOR_H

#include "block.h"

class Sector : public Block
{
    friend class Block;
public:
    virtual ~Sector() override;

    virtual QString title() const override;

    virtual void setDataSource(IDataSource *dataSource) override;

    virtual QString valueStr() const override;

    void append(Block * block);
    void insert(int index, Block * block);
    void replace(int index, Block * block);
    void remove(Block * block);
    const QList<Block *> & childs() const;

protected:
    void makeChild(Block * block);

private:
    QList<Block *> _childs;
};

#endif // SECTOR_H
