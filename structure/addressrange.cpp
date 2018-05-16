#include "addressrange.h"

AddressRange::AddressRange(QObject *parent)
    : AddressRange(parent, -1, -1)
{
}

AddressRange::AddressRange(QObject * parent, int begin, int end)
    : QObject(parent)
    , _begin(begin)
    , _end(end)
    , _style(nullptr)
    , _selected(false)
    , _visible(true)
    , _shadow(false)
{
    setStyle(new AreaStyle(this));
}

AddressRange::~AddressRange()
{
}

int AddressRange::begin() const { return _begin; }

void AddressRange::setBegin(int begin)
{
    if (_begin != begin){
        _begin = begin;
        updateIsSet();
        emit changed();
    }
}

int AddressRange::end() const { return _end; }

void AddressRange::setEnd(int end)
{
    if (_end != end){
        _end = end;
        updateIsSet();
        emit changed();
    }
}

int AddressRange::size() const {
    if (!isSet()) return 0;
    return _end - _begin + 1;
}

void AddressRange::reset()
{
    _begin = -1;
    _end = -1;

    updateIsSet();
    emit changed();
}

void AddressRange::updateIsSet()
{
    _isSet = _begin != -1 && _end != -1;
}

bool AddressRange::isSet() const
{
    return _isSet;
}

AreaStyle *AddressRange::style() const
{
    return _style;
}

void AddressRange::setStyle(AreaStyle *style)
{
    if (_style) _style->deleteLater();
    _style = style;
    _style->setParent(this);

    unselect();
}

void AddressRange::select()
{
    _selected = true;
    emit changed();
}

void AddressRange::unselect()
{
    _selected = false;
    emit changed();
}

bool AddressRange::selected() const
{
    return _selected;
}

void AddressRange::setShadow(bool value)
{
    if (_shadow == value) return;
    _shadow = value;
    emit changed();
}

bool AddressRange::shadow() const
{
    return _shadow;
}

void AddressRange::setName(const QString &value)
{
    if (_name == value) return;
    _name = value;
    emit changed();
}

const QString &AddressRange::name() const
{
    return _name;
}

bool AddressRange::isBlock() const
{
    return _isBlock;
}

void AddressRange::setIsBlock(bool isBlock)
{
    _isBlock = isBlock;
}

bool AddressRange::visible() const
{
    return _visible;
}

void AddressRange::setVisible(bool visible)
{
    if (_visible == visible) return;
    _visible = visible;
    emit changed();
}
