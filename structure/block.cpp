#include "block.h"

#include "sector.h"

Block::Block()
    : _dataSource(nullptr)
    , _parent(nullptr)
    , _offset(0)
    , _size(0)
    , _isValid(true)
    , _range(nullptr)
{
}

Block::~Block()
{
}

QString Block::title() const
{
    return QString("<i>%1</i> %2: <b>%3</b>").arg(_typeName).arg(_name).arg(_valueStr);
}

const QString & Block::typeName() const
{
    return _typeName;
}

void Block::setTypeName(const QString &typeName)
{
    _typeName = typeName;
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
    _offset = offset;
}

int Block::offset() const
{
    return _offset;
}

int Block::size() const
{
    return _size;
}

void Block::setSize(int value)
{
    Q_ASSERT(value >= 0);
    _size = value;
}

bool Block::isValid() const
{
    return _isValid;
}

const QString &Block::name() const
{
    return _name;
}

void Block::remove()
{
    Q_ASSERT(_parent != nullptr);

    _parent->remove(this);
}

Sector *Block::as_sector()
{
    return dynamic_cast<Sector*>(this);
}

AddressRange *Block::range()
{
    if (_range) return _range;

    AddressRange * range = new AddressRange(nullptr, _offset, _offset + _size - 1);
    range->setIsBlock(true);
    range->setStyle(new AreaStyle(QColor(0, 255, 0, 25), QColor(0, 255, 0), 2));

    return _range = range;
}

void Block::setName(const QString &name)
{
    _name = name;
}

void Block::setDescription(const QString &description)
{
    _description = description;
}

void Block::setValueStr(const QString &valueStr)
{
    _valueStr = valueStr;
}

QString Block::valueStr() const
{
    return _valueStr;
}
