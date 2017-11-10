#include "basedatainterpreter.h"

BaseDataInterpreter::BaseDataInterpreter()
    : QObject()
{
}

BaseDataInterpreter::~BaseDataInterpreter(){}

QString BaseDataInterpreter::value() const
{
    return "Value";
}
