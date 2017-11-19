#ifndef AREASTYLE_H
#define AREASTYLE_H

#include <QColor>

class AreaStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
public:
    explicit AreaStyle(QObject *parent = nullptr);
    AreaStyle(QObject *parent, QColor bgrColor, QColor borderColor, int borderWidth);

    QColor color() const;
    void setColor(QColor color);

    QColor borderColor() const;
    void setBorderColor(QColor color);

    int borderWidth() const;
    void setBorderWidth(int width);

signals:
    void colorChanged(QColor);
    void borderColorChanged(QColor);
    void borderWidthChanged(int);

public slots:
private:
    QColor _color;
    QColor _borderColor;
    int _borderWidth;
};

#endif // AREASTYLE_H
