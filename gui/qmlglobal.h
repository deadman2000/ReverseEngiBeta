#ifndef QMLROOT_H
#define QMLROOT_H

#include <QSize>
#include <QModelIndex>

class QmlGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize hexLetter MEMBER _hexLetter CONSTANT)
public:
    QmlGlobal();

signals:

public slots:

private:
    void updateFontSize();

private:
    QSize _hexLetter;
};

#endif // QMLROOT_H
