#include "addressrange.h"

AddressRange::AddressRange(QObject *parent)
    : QObject(parent)
    , _begin(-1)
    , _end(-1)
    , _isSet(false)
{
    _style = new AreaStyle(this);
}

AddressRange::AddressRange(QObject * parent, int begin, int end, QColor bgrColor, QColor borderColor, int borderWidth)
    : QObject(parent)
    , _begin(begin)
    , _end(end)
{
    _style = new AreaStyle(this, bgrColor, borderColor, borderWidth);
}

int AddressRange::begin() const { return _begin; }

void AddressRange::setBegin(int begin)
{
    if (_begin != begin){
        _begin = begin;
        emit beginChanged(begin);
        emit changed();
        updateIsSet();
        emit sizeChanged(size());
    }
}

int AddressRange::end() const { return _end; }

void AddressRange::setEnd(int end)
{
    if (_end != end){
        _end = end;
        emit endChanged(end);
        emit changed();
        updateIsSet();
        emit sizeChanged(size());
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

    emit changed();
    updateIsSet();
    emit sizeChanged(size());
}

void AddressRange::updateIsSet()
{
    bool oldIsSet = _isSet;
    _isSet = _begin != -1 && _end != -1;
    if (_isSet != oldIsSet)
        emit isSetChanged(_isSet);
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
    _style = style;
    emit styleChanged(style);
}
