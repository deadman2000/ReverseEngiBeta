#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include "basedatainterpreter.h"

class Instruments : public QObject
{
    Q_OBJECT
public:
    static Instruments & instance();

private:
    Q_DISABLE_COPY(Instruments)
    Instruments();
    ~Instruments();

public:
    QList<BaseDataInterpreter*> & interpreters();
    void registerInterpreter(BaseDataInterpreter * interpreter);

private:
    QList<BaseDataInterpreter*> _interpreters;
};

#endif // INSTRUMENTS_H
