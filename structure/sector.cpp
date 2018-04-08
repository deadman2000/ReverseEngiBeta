#include "sector.h"

Sector::~Sector()
{
    qDeleteAll(_childs);
}

QString Sector::title() const
{
    return _name;
}

void Sector::setDataSource(IDataSource *dataSource)
{
    Block::setDataSource(dataSource);
    for (Block* block : _childs)
    {
        block->setDataSource(dataSource);
    }
}

QString Sector::valueStr() const
{
    return QString();
}

void Sector::append(Block *block)
{
    _childs.append(block);
    makeChild(block);
}

void Sector::insert(int index, Block *block)
{
    _childs.insert(index, block);
    makeChild(block);
}

void Sector::replace(int index, Block *block)
{
    Q_ASSERT(index>= 0 && index < _childs.size());

    delete _childs[index];
    _childs[index] = block;
    makeChild(block);
}

void Sector::remove(Block *block)
{
    _childs.removeOne(block);
}

void Sector::makeChild(Block *block)
{
    block->setParent(this);
    block->setDataSource(_dataSource);
}

const QList<Block *> &Sector::childs() const
{
    return _childs;
}
