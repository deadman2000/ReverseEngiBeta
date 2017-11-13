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
    Q_PROPERTY(bool isSet READ isSet NOTIFY isSetChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
public:
    explicit AddressRange(QObject *parent = nullptr);
    AddressRange(QObject * parent, int begin, int end, QColor bgrColor, QColor borderColor, int borderWidth);

    int begin() const;
    void setBegin(int begin);

    int end() const;
    void setEnd(int end);

    int size() const;

    QColor color() const;
    void setColor(QColor color);

    QColor borderColor() const;
    void setBorderColor(QColor color);

    int borderWidth() const;
    void setBorderWidth(int width);

    bool isSet() const;

signals:
    void beginChanged(int);
    void endChanged(int);
    void sizeChanged(int);
    void changed();
    void isSetChanged(bool);

    void colorChanged(QColor);
    void borderColorChanged(QColor);
    void borderWidthChanged(int);

public slots:
    void reset();

private:
    void updateIsSet();

private:
    int _begin;
    int _end;
    bool _isSet;

    // TODO Move to class "Style"
    QColor _color;
    QColor _borderColor;
    int _borderWidth;
};

#endif // ADDRESSRANGE_H
