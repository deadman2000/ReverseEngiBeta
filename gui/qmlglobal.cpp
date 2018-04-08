#include "qmlglobal.h"

#include <QFontMetrics>
#include <QDir>

#include "treenode.h"

QmlGlobal::QmlGlobal() : QObject(nullptr)
{
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

void QmlGlobal::updateFontSize()
{
    QFont codeFont("CamingoCode");
    codeFont.setPixelSize(16);
    QString str(" ");
    QFontMetrics fm(codeFont);
    _hexLetter = fm.size(0, str);
}
