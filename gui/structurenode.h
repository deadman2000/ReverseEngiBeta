#ifndef STRUCTURENODE_H
#define STRUCTURENODE_H

#include "treenode.h"
#include "block.h"

class StructureNode : public TreeNode
{
    Q_OBJECT
    Q_PROPERTY(bool expanded READ expanded WRITE setExpanded NOTIFY expandedChanged)
public:
    explicit StructureNode(StructureNode * parent, Block * block);
    virtual ~StructureNode() override;

    Block * block() const;

    bool expanded() const;
    void setExpanded(bool expanded);

protected:
    void show();
    void hide();

public slots:
    virtual void update() override;
    AddressRange *range() const;

signals:
    void expandedChanged(bool);

private:
    Block * _block;
    bool _expanded;
};

#endif // STRUCTURENODE_H
