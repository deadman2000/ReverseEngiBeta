#ifndef STRUCTUREMODEL_H
#define STRUCTUREMODEL_H

#include <QAbstractListModel>

#include "block.h"
#include "sector.h"

class FileDocument;

class StructureModel : public QAbstractListModel
{
    Q_OBJECT

    enum Roles {
        AddressRole = Qt::UserRole + 1,
        HexRole,
        TextRole
    };
public:
    explicit StructureModel(FileDocument *doc);

    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    void append(structure::Block * block);

signals:

public slots:

private:
    FileDocument * _doc;
    structure::Sector _root;
};

#endif // STRUCTUREMODEL_H
