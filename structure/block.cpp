#include "block.h"

#include "sector.h"

static QList<QColor> PALETTE = {
    QColor(0xF44336),
    QColor(0xE91E63),
    QColor(0x9C27B0),
    QColor(0x673AB7),
    QColor(0x3F51B5),
    QColor(0x2196F3),
    QColor(0x03A9F4),
    QColor(0x00BCD4),
    QColor(0x009688),
    QColor(0x4CAF50),
    QColor(0x8BC34A),
    QColor(0xCDDC39),
    QColor(0xFFEB3B),
    QColor(0xFFC107),
    QColor(0xFF9800),
    QColor(0xFF5722),
    QColor(0x795548),
    QColor(0x9E9E9E),
    QColor(0x607D8B)
};
static int NEXT_COLOR_IND = 0;

QColor & getNextColor() {
    int c = NEXT_COLOR_IND;
    NEXT_COLOR_IND = (NEXT_COLOR_IND + 1) % PALETTE.size();
    return PALETTE[c];
}

Block::Block()
    : _dataSource(nullptr)
    , _parent(nullptr)
    , _offset(0)
    , _size(0)
    , _isValid(true)
    , _range(nullptr)
{
    _range.setIsBlock(true);

    QColor & color = getNextColor();
    QColor bgrColor = color;
    bgrColor.setAlpha(25);

    _range.setStyle(new AreaStyle(bgrColor, color, 2));
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
    _range.setBegin(offset);
}

int Block::offset() const
{
    return _offset;
}

void Block::setSize(int value)
{
    Q_ASSERT(value >= 0);
    _size = value;
    _range.setEnd(_offset + _size - 1);
}

int Block::size() const
{
    return _size;
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
    return &_range;
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
