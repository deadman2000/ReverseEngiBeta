#include "filedocument.h"

#include "structure.h"

FileDocument::FileDocument(QObject *parent)
    : QObject(parent)
{
    _data = new FileModel(this);
    //_structureModel = new StructureModel(this, _data);
    //connect(_structureModel, &StructureModel::sectionsChanged, this, &FileDocument::sectionsChanged);

    _structure.setDataSource(_data);
    _structure.setOffset(0);
}

FileDocument::~FileDocument()
{
    _data->deleteLater();
    //_structureModel->deleteLater();
}

void FileDocument::openFile(const QString &path)
{
    _fileName = path;
    _data->openFile(path);
    loadStructure("../../ReverseEngiBeta/win_exe.json");
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

void FileDocument::loadStructure(const QUrl &url)
{
    loadStructure(url.toLocalFile());
}

void FileDocument::loadStructure(const QString &path)
{
    _structure.load(path);
}

Sector* FileDocument::structure()
{
    return &_structure;
}
