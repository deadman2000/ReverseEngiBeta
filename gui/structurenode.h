#ifndef STRUCTURENODE_H
#define STRUCTURENODE_H

#include <QVariantMap>

#include "treenode.h"
#include "block.h"

class StructureNode : public TreeNode
{
    Q_OBJECT
public:
    explicit StructureNode(StructureNode * parent, Block * block);
    virtual ~StructureNode();

    Block * block() const;

public slots:
    virtual void update() override;
    QVariant data() const;

    virtual bool canAppend(TreeNode *node) const override;

    virtual void insert(int index, TreeNode *node) override;
    virtual void removeChild(TreeNode *node) override;

    void add(int typeId, const QVariantMap & attrs);
    void change(int typeId, const QVariantMap & attrs);

private:
    Sector * sector() const;

private:
    Block * _block;
};

#endif // STRUCTURENODE_H
