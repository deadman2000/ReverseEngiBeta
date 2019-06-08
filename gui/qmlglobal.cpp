#include "qmlglobal.h"

#include <QFontMetrics>
#include <QDir>

#include "treenode.h"

QmlGlobal::QmlGlobal()
    : QObject(nullptr)
{
    _rowHeight = 32;
    _mainFont = QFont("Hack");
    _mainFont.setPixelSize(16);
    updateFontSize();

    _tree = new TreeNode(nullptr);

    for (int i=0; i<5; i++)
    {
       TreeNode * node = new TreeNode();
       node->setText(QString("Node %1").arg(i));
       node->setIcon("qrc:icons/ic_folder_black_24px.svg");
       _tree->append(node);

       if (i % 2 == 0)
       {
           for (int j=0; j<5; j++)
           {
               TreeNode * subNode = new TreeNode();
               subNode->setText(QString("Sub node %1").arg(j));
               subNode->setIcon("qrc:icons/ic_folder_black_24px.svg");
               node->append(subNode);
           }
       }
    }
}

QObject *QmlGlobal::createStructure(FileDocument * file) const
{
    if (file->structure())
        return new StructureNode(nullptr, file->structure());
    return nullptr;
}

QColor QmlGlobal::blendAlpha(QColor color, qreal opacity) const
{
    return QColor(color.red(), color.green(), color.blue(), qRound(color.alpha() * opacity));
}

void QmlGlobal::updateFontSize()
{
    QString str(" ");
    QFontMetrics fm(_mainFont);
    _hexLetter = fm.size(0, str);
}
