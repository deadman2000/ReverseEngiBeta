#include "iengine.h"

#include "engine.h"

IEngine::IEngine()
{
}

IEngine::~IEngine()
{
}

IEngine &engine()
{
    static Engine engine;
    return engine;
}
