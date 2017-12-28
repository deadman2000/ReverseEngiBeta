#include "block.h"

#include "structure.h"

Block::Block()
    : _dataSource(nullptr)
    , _parent(nullptr)
    , _offset(-1)
    , _isValid(false)
    , _size(0)
{
}

Block::~Block()
{
}

void Block::setDataSource(IDataSource *dataSource)
{
    _dataSource = dataSource;
}

void Block::setParent(Sector *parent)
{
    _parent = parent;
}

Sector *Block::parent() const
{
    return _parent;
}

void Block::setOffset(int offset)
{
    if (_offset != offset){
        _offset = offset;
        _isValid = updateData();
    }
}

int Block::offset() const
{
    return _offset;
}

int Block::endOffset() const
{
    return _offset + _size - 1;
}

int Block::size() const
{
    return _size;
}

void Block::setSize(int value)
{
    Q_ASSERT(value >= 0);

    if (_size != value)
    {
        _size = value;
        if (_parent) _parent->updateData();
    }
}

bool Block::isValid() const
{
    return _isValid;
}

const QString &Block::name() const
{
    return _name;
}

void Block::acceptJSON(const QJsonObject &json)
{
    _name = json["name"].toString();
    readAttr(json);
}

void Block::readAttr(const QJsonObject &)
{
}

void Block::toJSON(QJsonObject &json) const
{
    json["name"] = _name;
    json["type"] = typeID();
    writeAttr(json);
}

void Block::writeAttr(QJsonObject &) const
{
}

void Block::save(QJsonObject &json) const
{
    toJSON(json);
}

void Block::remove()
{
    Q_ASSERT(_parent != nullptr);

    _parent->remove(this);
}

Block *Block::create(int typeId)
{
    switch (typeId)
    {
    case 0: return new Number();
    case 1: return new Text();
    case 2: return new Blob();
    case 3: return new Sector();
    default: throw new std::exception();
    }
}

void Block::setName(const QString &name)
{
    _name = name;
}

void Block::setDescription(const QString &description)
{
    _description = description;
}
