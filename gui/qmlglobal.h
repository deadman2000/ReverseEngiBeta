#ifndef QMLROOT_H
#define QMLROOT_H

#include <QSize>
#include <QUrl>

#include "treenode.h"
#include "filedocument.h"
#include "structurenode.h"

class QmlGlobal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize hexLetter MEMBER _hexLetter CONSTANT)
    Q_PROPERTY(TreeNode* tree MEMBER _tree CONSTANT)
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
};

#endif // QMLROOT_H
