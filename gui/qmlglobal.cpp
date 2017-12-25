#include "qmlglobal.h"

#include <QFontMetrics>

#include "treenode.h"

QmlGlobal::QmlGlobal() : QObject(nullptr)
{
    updateFontSize();

    _tree = new TreeModel(this);

    for (int i=0; i<5; i++)
    {
       TreeNode * node = _tree->createNode();
       node->setText(QString("Node %1").arg(i));
       node->setIcon("qrc:icons/ic_folder_black_24px.svg");

       if (i % 2 == 0)
       {
           for (int j=0; j<5; j++)
           {
               TreeNode * subNode = node->createChild();
               subNode->setText(QString("Sub node %1").arg(j));
               subNode->setIcon("qrc:icons/ic_folder_black_24px.svg");
           }
       }
    }
}

void QmlGlobal::updateFontSize()
{
    QFont codeFont("CamingoCode");
    codeFont.setPixelSize(16);
    QString str(" ");
    QFontMetrics fm(codeFont);
    _hexLetter = fm.size(0, str);
}
