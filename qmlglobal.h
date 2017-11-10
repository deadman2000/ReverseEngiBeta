#ifndef QMLROOT_H
#define QMLROOT_H

#include "instruments.h"

class QmlGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Instruments* instruments READ instruments CONSTANT)
public:
    QmlGlobal();

    Instruments * instruments() const;

signals:

public slots:
};

#endif // QMLROOT_H
