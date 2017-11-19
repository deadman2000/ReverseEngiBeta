#include "block.h"

namespace structure {

    Block::Block()
        : _dataSource(nullptr)
        , _offset(0)
    {
    }

    Block::~Block()
    {
    }

    void Block::setDataSource(IDataSource *dataSource)
    {
        _dataSource = dataSource;
    }

    void Block::setOffset(int offset)
    {
        _offset = offset;
        updateData();
    }

    bool Block::isValid() const
    {
        return true;
    }

    void Block::setName(const QString &name)
    {
        _name = name;
    }

    void Block::setDescription(const QString &description)
    {
        _description = description;
    }
}
