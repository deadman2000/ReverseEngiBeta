#ifndef QMLROOT_H
#define QMLROOT_H

#include <QSize>
#include <QUrl>
#include <QFont>

#include "treenode.h"
#include "filedocument.h"
#include "structurenode.h"

class QmlGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize hexLetter MEMBER _hexLetter CONSTANT)
    Q_PROPERTY(TreeNode* tree MEMBER _tree CONSTANT)
    Q_PROPERTY(QFont mainFont MEMBER _mainFont CONSTANT)
    Q_PROPERTY(qreal rowHeight MEMBER _rowHeight CONSTANT)
public:
    QmlGlobal();

signals:

public slots:
    QObject * createStructure(FileDocument * file) const;

private:
    void updateFontSize();

private:
    QSize _hexLetter;
    TreeNode* _tree;
    QFont _mainFont;
    qreal _rowHeight;
};

#endif // QMLROOT_H
