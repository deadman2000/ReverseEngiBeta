#include "treenode.h"

TreeNode::TreeNode(TreeNode *parent)
    : QAbstractListModel(parent)
{
}

TreeNode::~TreeNode()
{
    qDeleteAll(_nodes);
}

QString TreeNode::text() const
{
    return _text;
}

void TreeNode::setText(const QString &text)
{
    _text = text;
    emit textChanged(text);
}

QString TreeNode::icon() const
{
    return _icon;
}

void TreeNode::setIcon(const QString &icon)
{
    _icon = icon;
    emit iconChanged(icon);
}

TreeNode *TreeNode::parentNode() const
{
    return (TreeNode*)QObject::parent();
}

// Childs

QHash<int, QByteArray> TreeNode::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NodeRole] = "node";
    return roles;
}

int TreeNode::count() const
{
    return _nodes.count();
}

int TreeNode::rowCount(const QModelIndex &) const
{
    return _nodes.count();
}

QVariant TreeNode::data(const QModelIndex &index, int role) const
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

int TreeNode::indexOf(TreeNode *node) const
{
    return _nodes.indexOf(node);
}

void TreeNode::update()
{
    foreach (TreeNode * n, _nodes)
        n->update();
}

void TreeNode::append(TreeNode *node)
{
    Q_ASSERT(node);
    if (node->parentNode())
        node->remove();

    node->setParent(this);
    beginInsertRows(QModelIndex(), _nodes.count(), _nodes.count());
    _nodes.append(node);
    endInsertRows();

    emit countChanged(_nodes.count());
}

void TreeNode::insert(int index, TreeNode *node)
{
    Q_ASSERT(node);
    Q_ASSERT(index >= 0);
    node->setParent(this);
    beginInsertRows(QModelIndex(), index, index);
    _nodes.insert(index, node);
    endInsertRows();

    emit countChanged(_nodes.count());
}

void TreeNode::remove()
{
    parentNode()->removeChild(this);
}

void TreeNode::removeChild(TreeNode *node)
{
    Q_ASSERT(node);
    node->setParent(nullptr);
    int i = _nodes.indexOf(node);
    Q_ASSERT(i >= 0);
    beginRemoveRows(QModelIndex(), i, i);
    _nodes.removeAt(i);
    endRemoveRows();

    emit countChanged(_nodes.count());
}

void TreeNode::moveBefore(TreeNode *target)
{
    Q_ASSERT(target);
    remove();
    int i = target->parentNode()->indexOf(target);
    target->parentNode()->insert(i, this);
}

void TreeNode::moveAfter(TreeNode *target)
{
    Q_ASSERT(target);
    remove();
    int i = target->parentNode()->indexOf(target);
    target->parentNode()->insert(i + 1, this);
}

bool TreeNode::canAppend(TreeNode *) const
{
    return true;
}
