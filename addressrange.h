#ifndef ADDRESSRANGE_H
#define ADDRESSRANGE_H

#include "areastyle.h"

class AddressRange : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int begin READ begin WRITE setBegin NOTIFY beginChanged)
    Q_PROPERTY(int end READ end WRITE setEnd NOTIFY endChanged)
    Q_PROPERTY(bool isSet READ isSet NOTIFY isSetChanged)
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(AreaStyle* style READ style WRITE setStyle NOTIFY styleChanged)
public:
    explicit AddressRange(QObject *parent = nullptr);
    AddressRange(QObject * parent, int begin, int end, QColor bgrColor, QColor borderColor, int borderWidth);

    int begin() const;
    void setBegin(int begin);

    int end() const;
    void setEnd(int end);

    int size() const;

    bool isSet() const;

    AreaStyle * style() const;
    void setStyle(AreaStyle * style);

signals:
    void beginChanged(int);
    void endChanged(int);
    void sizeChanged(int);
    void changed();
    void isSetChanged(bool);
    void styleChanged(AreaStyle *);

public slots:
    void reset();

private:
    void updateIsSet();

private:
    int _begin;
    int _end;
    bool _isSet;
    AreaStyle * _style;
};

#endif // ADDRESSRANGE_H
