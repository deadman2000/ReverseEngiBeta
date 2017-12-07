#ifndef IENGINE_H
#define IENGINE_H

#include "basedatainterpreter.h"

class IEngine
{
public:
    IEngine();
    virtual ~IEngine();

    virtual QList<BaseDataInterpreter*> & interpreters() = 0;
    virtual void registerInterpreter(BaseDataInterpreter * interpreter) = 0;
};

IEngine & engine();

#endif // IENGINE_H
