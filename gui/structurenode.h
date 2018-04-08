#ifndef STRUCTURENODE_H
#define STRUCTURENODE_H

#include "treenode.h"
#include "block.h"

class StructureNode : public TreeNode
{
    Q_OBJECT
public:
    explicit StructureNode(StructureNode * parent, Block * block);
    virtual ~StructureNode() override;

    Block * block() const;

public slots:
    virtual void update() override;
    AddressRange *range() const;

private:
    Block * _block;
};

#endif // STRUCTURENODE_H
