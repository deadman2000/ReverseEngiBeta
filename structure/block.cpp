#include "block.h"

namespace structure {

    Block::Block()
        : _dataSource(nullptr)
        , _offset(-1)
        , _isValid(false)
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
        _isValid = updateData();
    }

    int Block::offset() const
    {
        return _offset;
    }

    int Block::endOffset() const
    {
        return _offset + _size - 1;
    }

    const QString &Block::name() const
    {
        return _name;
    }

    bool Block::isValid() const
    {
        return _isValid;
    }

    int Block::getSize() const
    {
        return _size;
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
