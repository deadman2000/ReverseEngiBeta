#include "qmlglobal.h"

QmlGlobal::QmlGlobal() : QObject(nullptr)
{

}

Instruments * QmlGlobal::instruments() const
{
    return &(Instruments::instance());
}
