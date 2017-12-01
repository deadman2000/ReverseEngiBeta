#include "structuremodel.h"

#include <QFile>
#include <QJsonDocument>

enum StructureRoles {
    StructureTypeRole = Qt::UserRole + 1,
    StructureValueRole,
    StructureBlockRole,
    StructureRangeRole
};

StructureModel::StructureModel(QObject * parent, IDataSource * dataSource)
    : QStandardItemModel(parent)
    , _selectedRange(nullptr)
{
    QHash<int, QByteArray> roles;
    roles[StructureTypeRole] = "type";
    roles[Qt::DisplayRole] = "name";
    roles[StructureValueRole] = "value";
    setItemRoleNames(roles);

    _root.setDataSource(dataSource);
    _root.setOffset(0);


    // TODO Потом убрать
    /*{
        auto * b = new structure::Text();
        b->setName("Signature");
        b->setSize(2);
        _root.append(b);
    }
    for (int i=1; i<=5; i++)
    {
        auto * b = new structure::Number();
        b->setName(QString("some_value_%1").arg(i));
        b->setSize(2);
        _root.append(b);
    }
    {
        auto * s = new structure::Sector();
        s->setName("Sector");
        _root.append(s);

        for (int i=1; i<=5; i++)
        {
            auto * b = new structure::Number();
            b->setName(QString("sub_value_%1").arg(i));
            b->setSize(4);
            s->append(b);
        }
    }
    buildStructure();

    saveStructure("win_exe.json");*/
}

void StructureModel::buildStructure()
{
    clear();
    //qDeleteAll(_blocks); // Падает
    _blocks.clear();
    addSectorToTree(_root, invisibleRootItem());
    emit sectionsChanged();
}

void StructureModel::addSectorToTree(structure::Sector & s, QStandardItem *parentItem)
{
    for (structure::Block* b : s.childs())
    {
        QStandardItem *item = new QStandardItem(b->name());
        item->setData(b->toString(), StructureValueRole);
        item->setData(b->typeName(), StructureTypeRole);
        item->setData(qVariantFromValue((void*)b), StructureBlockRole);
        parentItem->appendRow(item);

        if (structure::Sector * s = dynamic_cast<structure::Sector*>(b))
            addSectorToTree(*s, item);
        else
        {
            AddressRange* range = new AddressRange(this, b->offset(), b->endOffset());
            range->setStyle(new AreaStyle(QColor(0, 255, 0, 25), QColor(0, 255, 0), 1));
            connect(range, &AddressRange::changed, this, &StructureModel::sectionsChanged);

            _blocks.append(range);
            item->setData(qVariantFromValue((void*)range), StructureRangeRole);
        }
    }
}

QQmlListProperty<AddressRange> StructureModel::blocks()
{
    return QQmlListProperty<AddressRange>(this, _blocks);
}

QVariant StructureModel::getBlock(QModelIndex index)
{
    QVariantMap map;

    QStandardItem * item = itemFromIndex(index);
    if (!item) return map;

    structure::Block* b = (structure::Block*)item->data(StructureBlockRole).value<void*>();

    {
        QJsonObject obj;
        b->toJSON(obj);
        foreach (const QString & key, obj.keys())
        {
            map[key] = obj[key].toVariant();
        }
    }

    return QVariant::fromValue(map);
}

bool StructureModel::saveStructure(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject root;
    _root.save(root);

    QJsonDocument doc(root);
    file.write(doc.toJson());

    return true;
}

bool StructureModel::loadStructure(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
    _root.acceptJSON(doc.object());

    buildStructure();

    return true;
}

void StructureModel::addBlock(QModelIndex parentIndex, int type, const QVariantMap &attrs)
{
    structure::Sector * parent = nullptr;
    if (parentIndex.isValid())
    {
        QStandardItem * item = itemFromIndex(parentIndex);
        structure::Block* b = (structure::Block*)item->data(StructureBlockRole).value<void*>();
        parent = dynamic_cast<structure::Sector*>(b);
        if (!parent)
            parent = b->parent();
    }
    if (!parent) parent = &_root;

    structure::Block * block = structure::Block::create(type);
    QJsonDocument json(QJsonDocument::fromVariant(attrs));
    block->acceptJSON(json.object());

    parent->append(block);
    buildStructure();
}

void StructureModel::editBlock(QModelIndex index, int type, const QVariantMap &attrs)
{
    Q_ASSERT(index.isValid());

    QStandardItem * item = itemFromIndex(index);
    auto * block = (structure::Block*)item->data(StructureBlockRole).value<void*>();

    if (block->typeID() != type)
    {
        auto * old = block;
        auto * parent = block->parent();
        int ind = parent->childs().indexOf(block);
        Q_ASSERT(ind != -1);
        block = structure::Block::create(type);
        parent->replace(ind, block);
        //delete old; // Падает при удалении блока
    }

    QJsonDocument json(QJsonDocument::fromVariant(attrs));
    block->acceptJSON(json.object());

    buildStructure();
}

void StructureModel::deleteBlock(QModelIndex index)
{
    Q_ASSERT(index.isValid());

    QStandardItem * item = itemFromIndex(index);
    auto * block = (structure::Block*)item->data(StructureBlockRole).value<void*>();

    block->remove();

    buildStructure();
}

void StructureModel::selectBlock(QModelIndex index)
{
    if (index.isValid())
    {
        QStandardItem * item = itemFromIndex(index);
        //_currentBlock = (structure::Block*)item->data(StructureBlockRole).value<void*>();
        AddressRange* range = (AddressRange*)item->data(StructureRangeRole).value<void*>();
        selectRange(range);
    }
    else
    {
        selectRange(nullptr);
    }
}

void StructureModel::selectRange(AddressRange *range)
{
    if (_selectedRange)
        _selectedRange->unselect();
    _selectedRange = range;
    if (range)
        range->select();
}
