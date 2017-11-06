#include "addressrange.h"

AddressRange::AddressRange(QObject *parent)
    : QObject(parent)
    , _begin(-1)
    , _end(-1)
{

}

void AddressRange::setBegin(int begin)
{
    if (_begin != begin){
        _begin = begin;
        emit beginChanged(begin);
        emit changed();
    }
}

void AddressRange::setEnd(int end)
{
    if (_end != end){
        _end = end;
        emit endChanged(end);
        emit changed();
    }
}

void AddressRange::reset()
{
    _begin = -1;
    _end = -1;

    emit changed();
}

bool AddressRange::isSet() const
{
    return _begin != -1 && _end != -1;
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
