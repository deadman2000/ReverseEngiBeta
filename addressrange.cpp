#include "addressrange.h"

AddressRange::AddressRange(QObject *parent)
    : QObject(parent)
    , _begin(-1)
    , _end(-1)
    , _isSet(false)
    , _style(nullptr)
    , _selectedStyle(nullptr)
    , _selected(false)
{
    setStyle(new AreaStyle(this));

    _selectedStyle = new AreaStyle(this);
    _selectedStyle->setColor(QColor(255, 0, 0, 25));
}

AddressRange::AddressRange(QObject * parent, int begin, int end)
    : QObject(parent)
    , _begin(begin)
    , _end(end)
    , _style(nullptr)
    , _selectedStyle(nullptr)
    , _selected(false)
{
    setStyle(new AreaStyle(this));

    _selectedStyle = new AreaStyle(this);
    _selectedStyle->setColor(QColor(255, 0, 0, 25));
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
    if (_selected)
        return _selectedStyle;
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
