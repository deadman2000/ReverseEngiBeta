#include "structurenode.h"

#include "block.h"
#include "sector.h"

StructureNode::StructureNode(StructureNode * parent, Block * block)
    : TreeNode(parent)
    , _block(block)
    , _expanded(false)
{
    update();

    Sector * s = block->as_sector();
    if (s){
        foreach (Block* b, s->childs())
            _nodes.append(new StructureNode(this, b));
    }

    if (!parent)
    {
        _expanded = true;
        show();
    }
}

StructureNode::~StructureNode()
{
}

Block *StructureNode::block() const
{
    return _block;
}

bool StructureNode::expanded() const
{
    return _expanded;
}

void StructureNode::setExpanded(bool expanded)
{
    _expanded = expanded;
    show();
    emit expandedChanged(expanded);
}

void StructureNode::show()
{
    if (_expanded &&  _nodes.count() > 0)
    {
        _block->range()->setVisible(false);
        for (TreeNode* n : _nodes){
            ((StructureNode*) n)->show();
        }
    } else {
        _block->range()->setVisible(true);
        for (TreeNode* n : _nodes){
            ((StructureNode*) n)->hide();
        }
    }
}

void StructureNode::hide()
{
    _block->range()->setVisible(false);
    for (TreeNode* n : _nodes){
        ((StructureNode*) n)->hide();
    }
}

void StructureNode::update()
{
    setText(_block->title());
    TreeNode::update();
}

AddressRange * StructureNode::range() const
{
    return _block->range();
}
