#include "structuremodel.h"
#include "filedocument.h"

StructureModel::StructureModel(FileDocument *doc)
    : QAbstractListModel(doc)
    , _doc(doc)
{
}


QHash<int, QByteArray> StructureModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[AddressRole] = "address";
    roles[HexRole] = "hex";
    roles[TextRole] = "fileText";
    return roles;
}

int StructureModel::rowCount(const QModelIndex &parent) const
{
    return _root.count();
}

QVariant StructureModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

void StructureModel::append(structure::Block *block)
{
    _root.append(block);
}
