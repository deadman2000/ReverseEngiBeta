#ifndef ENGINE_H
#define ENGINE_H

#include "iengine.h"

class Engine : public IEngine
{
public:
    Engine();

    virtual QList<BaseDataInterpreter *> &interpreters() override;
    virtual void registerInterpreter(BaseDataInterpreter *interpreter) override;

private:
    QList<BaseDataInterpreter*> _interpreters;
};

#endif // ENGINE_H
