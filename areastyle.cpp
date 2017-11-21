#include "areastyle.h"

AreaStyle::AreaStyle(QObject *parent)
    : QObject(parent)
    , _color(Qt::transparent)
    , _borderColor(Qt::black)
    , _borderWidth(2)
{
}

AreaStyle::AreaStyle(QColor bgrColor)
    : _color(bgrColor)
    , _borderColor(Qt::transparent)
    , _borderWidth(0)
{
}

AreaStyle::AreaStyle(QColor bgrColor, QColor borderColor, int borderWidth)
    : _color(bgrColor)
    , _borderColor(borderColor)
    , _borderWidth(borderWidth)
{
}

QColor AreaStyle::color() const
{
    return _color;
}

void AreaStyle::setColor(QColor color)
{
    _color = color;
    emit colorChanged(color);
}

QColor AreaStyle::borderColor() const
{
    return _borderColor;
}

void AreaStyle::setBorderColor(QColor color)
{
    _borderColor = color;
    emit borderColorChanged(color);
}

int AreaStyle::borderWidth() const
{
    return _borderWidth;
}

void AreaStyle::setBorderWidth(int width)
{
    _borderWidth = width;
    emit borderWidthChanged(width);
}
