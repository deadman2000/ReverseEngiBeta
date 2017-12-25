#include "treenode.h"

#include "treemodel.h"

TreeNode::TreeNode(TreeModel *parent)
    : QObject(parent)
    , _model(parent)
    , _children(nullptr)
{
}

TreeNode::~TreeNode()
{
    qDebug() << "Delete" << _text;
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

TreeModel *TreeNode::children() const
{
    return _children;
}

void TreeNode::appendNode(TreeNode *obj)
{
    if (!_children) {
        _children = new TreeModel(this);
    }
    _children->appendNode(obj);
    emit childrenChanged(_children);
}

TreeNode *TreeNode::createChild()
{
    if (!_children) {
        _children = new TreeModel(this);
    }
    return _children->createNode();
}

TreeModel *TreeNode::model() const
{
    return _model;
}

void TreeNode::setModel(TreeModel *model)
{
    Q_ASSERT(model);
    setParent(model);
    _model = model;
}

void TreeNode::moveBefore(TreeNode *target)
{
    Q_ASSERT(target);
    _model->remove(this);
    int i = target->model()->indexOf(target);
    target->model()->insert(i, this);
}

void TreeNode::moveAfter(TreeNode *target)
{
    Q_ASSERT(target);
    _model->remove(this);
    int i = target->model()->indexOf(target);
    target->model()->insert(i + 1, this);
}
