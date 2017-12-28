#include "structurenode.h"

#include <QJsonDocument>
#include "structure.h"

StructureNode::StructureNode(StructureNode * parent, Block * block)
    : TreeNode(parent)
    , _block(block)
{
    update();

    Sector * s = sector();
    if (s){
        foreach (Block* b, s->childs())
            _nodes.append(new StructureNode(this, b));
    }
}

StructureNode::~StructureNode()
{
}

Block *StructureNode::block() const
{
    return _block;
}

void StructureNode::update()
{
    setText(QString("<i>%1</i> %2: <b>%3</b>").arg(_block->typeName()).arg(_block->name()).arg(_block->toString()));
    TreeNode::update();
}

bool StructureNode::canAppend(TreeNode *) const
{
    return sector() != nullptr;
}

void StructureNode::insert(int index, TreeNode *node)
{
    TreeNode::insert(index, node);

    sector()->insert(index, static_cast<StructureNode*>(node)->block());
}

void StructureNode::removeChild(TreeNode *node)
{
    TreeNode::removeChild(node);

    sector()->remove(static_cast<StructureNode*>(node)->block());
}

void StructureNode::add(int typeId, const QVariantMap &attrs)
{
    Block * block = Block::create(typeId);
    QJsonDocument json(QJsonDocument::fromVariant(attrs));
    block->acceptJSON(json.object());

    if (sector())
    {
        sector()->append(block);
        _nodes.append(new StructureNode(this, block));
    }
    else
    {
        _block->parent()->append(block);
        parentNode()->append(new StructureNode(nullptr, block));
    }
}

void StructureNode::change(int typeId, const QVariantMap &attrs)
{
    if (_block->typeID() != typeId)
    {
        auto * parent = _block->parent();
        int ind = parent->childs().indexOf(_block);
        Q_ASSERT(ind != -1);
        _block = Block::create(typeId);
        parent->replace(ind, _block);
    }

    QJsonDocument json(QJsonDocument::fromVariant(attrs));
    _block->acceptJSON(json.object());
}

QVariant StructureNode::data() const
{
    QVariantMap map;

    QJsonObject obj;
    _block->toJSON(obj);
    foreach (const QString & key, obj.keys())
        map[key] = obj[key].toVariant();

    return QVariant::fromValue(map);
}


Sector *StructureNode::sector() const
{
    return dynamic_cast<Sector*>(_block);
}
