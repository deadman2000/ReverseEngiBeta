#include "instruments.h"

Instruments &Instruments::instance() {
    static Instruments instance;
    return instance;
}

Instruments::Instruments()
    : QObject()
{
}

Instruments::~Instruments()
{
}

QList<BaseDataInterpreter *> &Instruments::interpreters()
{
    return _interpreters;
}

void Instruments::registerInterpreter(BaseDataInterpreter *interpreter)
{
    _interpreters.append(interpreter);
}
