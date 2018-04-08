#include "structurenode.h"

#include "block.h"
#include "sector.h"

StructureNode::StructureNode(StructureNode * parent, Block * block)
    : TreeNode(parent)
    , _block(block)
{
    update();

    Sector * s = block->as_sector();
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
    setText(_block->title());
    TreeNode::update();
}

AddressRange * StructureNode::range() const
{
    return _block->range();
}
