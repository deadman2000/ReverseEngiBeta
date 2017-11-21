#include "filedocument.h"

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
    {
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
    addSectorToTree(_structure, _structureModel->invisibleRootItem());
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
        //structure::Block* b = (structure::Block*)item->data(StructureBlockRole).value<void*>();
        AddressRange* range = (AddressRange*)item->data(StructureRangeRole).value<void*>();
        selectRange(range);
    }
    else
    {
        selectRange(nullptr);
    }
}

void FileDocument::selectRange(AddressRange *range)
{
    if (_selectedRange)
        _selectedRange->unselect();
    _selectedRange = range;
    if (range)
        range->select();
}
