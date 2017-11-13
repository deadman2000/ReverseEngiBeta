#ifndef QMLROOT_H
#define QMLROOT_H

#include "instruments.h"

#include <QSize>

class QmlGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Instruments* instruments READ instruments CONSTANT)
    Q_PROPERTY(QSize hexLetter MEMBER _hexLetter CONSTANT)
public:
    QmlGlobal();

    Instruments * instruments() const;

signals:

public slots:

private:
    void updateFontSize();

private:
    QSize _hexLetter;
};

#endif // QMLROOT_H
