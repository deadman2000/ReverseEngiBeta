#include "engine.h"

Engine::Engine()
{
}

QList<BaseDataInterpreter *> &Engine::interpreters()
{
    return _interpreters;
}

void Engine::registerInterpreter(BaseDataInterpreter *interpreter)
{
    _interpreters.append(interpreter);
}
