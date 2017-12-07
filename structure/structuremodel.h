#ifndef STRUCTUREMODEL_H
#define STRUCTUREMODEL_H

#include <QStandardItemModel>
#include <QQmlListProperty>

#include "structure.h"
#include "addressrange.h"

class StructureModel : public QStandardItemModel
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AddressRange> blocks READ blocks CONSTANT)
public:
    StructureModel(QObject * parent, IDataSource * dataSource);

    void buildStructure();

    QQmlListProperty<AddressRange> blocks();

    Q_INVOKABLE void addBlock(QModelIndex parentIndex, int type, const QVariantMap & attrs);
    Q_INVOKABLE void editBlock(QModelIndex index, int type, const QVariantMap & attrs);
    Q_INVOKABLE void deleteBlock(QModelIndex index);

    Q_INVOKABLE void selectBlock(QModelIndex index);

    Q_INVOKABLE QVariant getBlock(QModelIndex index);

    Q_INVOKABLE bool saveStructure(const QString & fileName);
    Q_INVOKABLE bool loadStructure(const QString & fileName);

private:
    void addSectorToTree(Sector & s, QStandardItem *parentItem);
    void selectRange(AddressRange * range);

signals:
    void sectionsChanged();

private:
    Sector _root;
    QList<AddressRange*> _blocks;
    AddressRange* _selectedRange;
};

#endif // STRUCTUREMODEL_H
