#include "sector.h"

namespace structure {

    Sector::Sector()
    {
    }

    Sector::~Sector()
    {
        qDeleteAll(_childs);
    }

    QString structure::Sector::toString() const
    {
        return QString();
    }

    int structure::Sector::getSize() const
    {
        return 0;
    }

    int Sector::count() const
    {
        return _childs.count();
    }

    void Sector::append(Block *block)
    {
        block->setDataSource(_dataSource);
        block->setOffset(0); // TODO Calc offset
        _childs.append(block);
    }

    void structure::Sector::updateData()
    {
    }
}
