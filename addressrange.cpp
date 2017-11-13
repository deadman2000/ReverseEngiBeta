#include "addressrange.h"

AddressRange::AddressRange(QObject *parent)
    : QObject(parent)
    , _begin(-1)
    , _end(-1)
    , _isSet(false)
{
}

AddressRange::AddressRange(QObject * parent, int begin, int end, QColor bgrColor, QColor borderColor, int borderWidth)
    : QObject(parent)
    , _begin(begin)
    , _end(end)
    , _color(bgrColor)
    , _borderColor(borderColor)
    , _borderWidth(borderWidth)
{
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

QColor AddressRange::color() const
{
    return _color;
}

void AddressRange::setColor(QColor color)
{
    _color = color;
    emit colorChanged(color);
}

QColor AddressRange::borderColor() const
{
    return _borderColor;
}

void AddressRange::setBorderColor(QColor color)
{
    _borderColor = color;
    emit borderColorChanged(color);
}

int AddressRange::borderWidth() const
{
    return _borderWidth;
}

void AddressRange::setBorderWidth(int width)
{
    _borderWidth = width;
    emit borderWidthChanged(width);
}
