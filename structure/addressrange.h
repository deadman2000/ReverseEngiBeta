#ifndef ADDRESSRANGE_H
#define ADDRESSRANGE_H

#include "areastyle.h"

class AddressRange : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int begin READ begin WRITE setBegin NOTIFY changed)
    Q_PROPERTY(int end READ end WRITE setEnd NOTIFY changed)
    Q_PROPERTY(bool isSet READ isSet NOTIFY changed)
    Q_PROPERTY(int size READ size NOTIFY changed)
    Q_PROPERTY(AreaStyle* style READ style WRITE setStyle NOTIFY changed)
public:
    explicit AddressRange(QObject *parent = nullptr);
    AddressRange(QObject * parent, int begin, int end);
    ~AddressRange();

    int begin() const;
    void setBegin(int begin);

    int end() const;
    void setEnd(int end);

    int size() const;

    bool isSet() const;

    AreaStyle * style() const;
    void setStyle(AreaStyle * style);

    bool isBlock() const;
    void setIsBlock(bool isBlock);

signals:
    void changed();

public slots:
    void reset();

    void select();
    void unselect();

private:
    void updateIsSet();

private:
    int _begin;
    int _end;
    bool _isSet;
    bool _isBlock;

    AreaStyle *_style, *_selectedStyle;
    bool _selected;
};

#endif // ADDRESSRANGE_H
