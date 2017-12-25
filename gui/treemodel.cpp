#include "treemodel.h"

#include "treenode.h"

TreeModel::TreeModel(QObject *parent)
    : QAbstractListModel(parent)
    , _parent(nullptr)
{

}

TreeModel::TreeModel(TreeNode * node)
    : QAbstractListModel(node)
    , _parent(node)
{
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NodeRole] = "node";
    return roles;
}

bool TreeModel::hasChilds() const
{
    return _nodes.count() > 0;
}

int TreeModel::rowCount(const QModelIndex &) const
{
    return _nodes.count();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _nodes.count() || index.row() < 0)
        return QVariant();

    TreeNode * obj = _nodes.at(index.row());
    switch (role)
    {
    case NodeRole:
        return QVariant::fromValue(obj);
    }

    return QVariant();
}

int TreeModel::indexOf(TreeNode *node) const
{
    return _nodes.indexOf(node);
}

TreeNode *TreeModel::createNode()
{
    TreeNode * node = new TreeNode();
    appendNode(node);
    return node;
}

void TreeModel::appendNode(TreeNode *node)
{
    Q_ASSERT(node);
    node->setModel(this);
    beginInsertRows(QModelIndex(), _nodes.count(), _nodes.count());
    _nodes.append(node);
    endInsertRows();

    if (_nodes.count() == 1)
        emit hasChildsChanged(true);
}

void TreeModel::insert(int index, TreeNode *node)
{
    node->setModel(this);
    beginInsertRows(QModelIndex(), index, index);
    _nodes.insert(index, node);
    endInsertRows();

    if (_nodes.count() == 1)
        emit hasChildsChanged(true);
}

void TreeModel::remove(TreeNode *node)
{
    node->setModel(this);
    int index = _nodes.indexOf(node);
    beginRemoveRows(QModelIndex(), index, index);
    _nodes.removeAt(index);
    endRemoveRows();

    if (_nodes.count() == 0)
        emit hasChildsChanged(false);
}
