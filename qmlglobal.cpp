#include "qmlglobal.h"

#include <QFontMetrics>

QmlGlobal::QmlGlobal() : QObject(nullptr)
{
    updateFontSize();
}

Instruments * QmlGlobal::instruments() const
{
    return &(Instruments::instance());
}

void QmlGlobal::updateFontSize()
{
    QFont codeFont("CamingoCode");
    codeFont.setPixelSize(16);
    QString str(" ");
    QFontMetrics fm(codeFont);
    _hexLetter = fm.size(0, str);
}
