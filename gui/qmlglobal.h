#ifndef QMLROOT_H
#define QMLROOT_H

#include <QSize>
#include <QModelIndex>
#include "treemodel.h"

class QmlGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize hexLetter MEMBER _hexLetter CONSTANT)
    Q_PROPERTY(TreeModel* tree MEMBER _tree CONSTANT)
public:
    QmlGlobal();

signals:

public slots:

private:
    void updateFontSize();

private:
    QSize _hexLetter;
    TreeModel* _tree;
};

#endif // QMLROOT_H
