#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractListModel>

class TreeNode;

class TreeModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(bool hasChilds READ hasChilds NOTIFY hasChildsChanged)
    enum TreeRoles {
        NodeRole = Qt::UserRole + 1
    };
public:
    explicit TreeModel(QObject * parent = nullptr);
    explicit TreeModel(TreeNode * node);

    virtual QHash<int, QByteArray> roleNames() const override;

    bool hasChilds() const;

    virtual int rowCount(const QModelIndex &parent) const override;

    virtual QVariant data(const QModelIndex &index, int role) const override;

    int indexOf(TreeNode * node) const;

    TreeNode * createNode();
    void appendNode(TreeNode * node);
    void insert(int index, TreeNode * node);
    void remove(TreeNode * node);

signals:
    void hasChildsChanged(bool);

private:
    TreeNode * _parent;
    QList<TreeNode*> _nodes;
};

#endif // TREEMODEL_H
