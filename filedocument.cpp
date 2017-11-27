#include "filedocument.h"

#include <QFile>
#include <QJsonDocument>

#include "structure.h"

enum StructureRoles {
    StructureValueRole = Qt::UserRole + 1,
    StructureBlockRole,
    StructureRangeRole
};

FileDocument::FileDocument(QObject *parent)
    : QObject(parent)
    , _selectedRange(nullptr)
{
    _data = new FileModel(this);
    _structure.setDataSource(_data);
    _structure.setOffset(0);

    _structureModel = new QStandardItemModel(this);
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[StructureValueRole] = "value";
    _structureModel->setItemRoleNames(roles);
}

FileDocument::~FileDocument()
{
    _data->deleteLater();
    _structureModel->deleteLater();
}

void FileDocument::openFile(const QString &path)
{
    _fileName = path;
    _data->openFile(path);

    // TODO Потом убрать
    /*{
        auto * b = new structure::Text();
        b->setName("Signature");
        b->setSize(2);
        _structure.append(b);
    }
    for (int i=1; i<=5; i++)
    {
        auto * b = new structure::Number();
        b->setName(QString("some_value_%1").arg(i));
        b->setSize(2);
        _structure.append(b);
    }
    {
        auto * s = new structure::Sector();
        s->setName("Sector");
        _structure.append(s);

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

    loadStructure("win_exe.json");
}

const QString & FileDocument::fileName() const
{
    return _fileName;
}

FileModel *FileDocument::data() const
{
    return _data;
}

QQmlListProperty<AddressRange> FileDocument::sections()
{
    return QQmlListProperty<AddressRange>(this, _sections);
}

void FileDocument::addSection(int begin, int end)
{
    AddressRange * section = new AddressRange(this, begin, end);
    section->setStyle(new AreaStyle(QColor(0, 255, 0, 25), QColor(0, 255, 0), 2));
    _sections.append(section);
    connect(section, &AddressRange::changed, this, &FileDocument::sectionsChanged);
}

void FileDocument::addSectorToTree(structure::Sector & s, QStandardItem *parentItem)
{
    for (structure::Block* b : s.childs())
    {
        QStandardItem *item = new QStandardItem(b->name());
        item->setData(b->toString(), StructureValueRole);
        item->setData(qVariantFromValue((void*)b), StructureBlockRole);
        parentItem->appendRow(item);

        if (structure::Sector * s = dynamic_cast<structure::Sector*>(b))
            addSectorToTree(*s, item);
        else
        {
            AddressRange* range = new AddressRange(this, b->offset(), b->endOffset());
            range->setStyle(new AreaStyle(QColor(0, 255, 0, 25), QColor(0, 255, 0), 1));
            connect(range, &AddressRange::changed, this, &FileDocument::sectionsChanged);

            _blocks.append(range);
            item->setData(qVariantFromValue((void*)range), StructureRangeRole);
        }
    }
}

void FileDocument::buildStructure()
{
    _structureModel->clear();
    _blocks.clear();
    addSectorToTree(_structure, _structureModel->invisibleRootItem());
    emit sectionsChanged();
}

QStandardItemModel *FileDocument::structure() const
{
    return _structureModel;
}

QQmlListProperty<AddressRange> FileDocument::blocks()
{
    return QQmlListProperty<AddressRange>(this, _blocks);
}

void FileDocument::selectBlock(QModelIndex index)
{
    if (index.isValid())
    {
        QStandardItem * item = _structureModel->itemFromIndex(index);
        //_currentBlock = (structure::Block*)item->data(StructureBlockRole).value<void*>();

        AddressRange* range = (AddressRange*)item->data(StructureRangeRole).value<void*>();
        selectRange(range);
    }
    else
    {
        selectRange(nullptr);
    }
}

QVariant FileDocument::getBlock(QModelIndex index)
{
    QVariantMap map;

    QStandardItem * item = _structureModel->itemFromIndex(index);
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

bool FileDocument::saveStructure(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject root;
    _structure.save(root);

    QJsonDocument doc(root);
    file.write(doc.toJson());

    return true;
}

bool FileDocument::loadStructure(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
    _structure.acceptJSON(doc.object());

    buildStructure();

    return true;
}

void FileDocument::addBlock(QModelIndex parentIndex, int type, const QVariantMap &attrs)
{
    structure::Sector * parent = nullptr;
    if (parentIndex.isValid())
    {
        QStandardItem * item = _structureModel->itemFromIndex(parentIndex);
        structure::Block* b = (structure::Block*)item->data(StructureBlockRole).value<void*>();
        parent = dynamic_cast<structure::Sector*>(b);
        if (!parent)
            parent = b->parent();
    }
    if (!parent) parent = &_structure;

    structure::Block * block = structure::Block::create(type);
    QJsonDocument json(QJsonDocument::fromVariant(attrs));
    block->acceptJSON(json.object());

    parent->append(block);
    buildStructure();
}

void FileDocument::editBlock(QModelIndex index, int type, const QVariantMap &attrs)
{
    Q_ASSERT(index.isValid());

    QStandardItem * item = _structureModel->itemFromIndex(index);
    auto * block = (structure::Block*)item->data(StructureBlockRole).value<void*>();

    if (block->typeID() != type)
    {
        auto * old = block;
        auto * parent = block->parent();
        int ind = parent->childs().indexOf(block);
        Q_ASSERT(ind != -1);
        block = structure::Block::create(type);
        parent->replace(ind, block);
    }

    QJsonDocument json(QJsonDocument::fromVariant(attrs));
    block->acceptJSON(json.object());

    buildStructure();
}

void FileDocument::selectRange(AddressRange *range)
{
    if (_selectedRange)
        _selectedRange->unselect();
    _selectedRange = range;
    if (range)
        range->select();
}
