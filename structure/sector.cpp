#include "sector.h"

namespace structure {

    Sector::Sector()
    {
    }

    Sector::~Sector()
    {
        qDeleteAll(_childs);
    }

    QString Sector::toString() const
    {
        return QString();
    }

    void Sector::append(Block *block)
    {
        _childs.append(block);

        block->setDataSource(_dataSource);
        if (_isValid && _offset >= 0)
        {
            block->setOffset(_offset + _size);
            if (block->isValid())
                _size += block->getSize();
            else
                _isValid = false;
        }
    }

    const QList<Block *> &Sector::childs() const
    {
        return _childs;
    }

    bool Sector::updateData()
    {
        _size = 0;
        for (Block* block : _childs)
        {
            block->setOffset(_offset + _size);
            if (block->isValid())
                _size += block->getSize();
            else
                return false;
        }
        return true;
    }
}
