#ifndef ADDRESSRANGE_H
#define ADDRESSRANGE_H

#include <QObject>
#include <QColor>

class AddressRange : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int begin READ begin WRITE setBegin NOTIFY beginChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(int end READ end WRITE setEnd NOTIFY endChanged)
public:
    explicit AddressRange(QObject *parent = nullptr);

    int begin() const { return _begin; }

    void setBegin(int begin);

    int end() const { return _end; }

    void setEnd(int end);

    QColor color() const;
    void setColor(QColor color);

    QColor borderColor() const;
    void setBorderColor(QColor color);

    int borderWidth() const;
    void setBorderWidth(int width);

signals:
    void beginChanged(int);
    void endChanged(int);
    void changed();

    void colorChanged(QColor);
    void borderColorChanged(QColor);
    void borderWidthChanged(int);

public slots:
    void reset();
    bool isSet() const;

private:
    int _begin;
    int _end;
    QColor _color;
    QColor _borderColor;
    int _borderWidth;
};

#endif // ADDRESSRANGE_H
