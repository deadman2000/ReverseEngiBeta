#include "filedocument.h"

#include "struct_reader.h"

FileDocument::FileDocument(QObject *parent)
    : QObject(parent)
{
    _data = new FileModel(this);
}

FileDocument::~FileDocument()
{
    _data->deleteLater();
    delete _structure;
    //_structureModel->deleteLater();
}

void FileDocument::openFile(const QString &path)
{
    if (path.startsWith("file://")){
        QUrl url(path);
        if (url.isValid())
            _fileName = url.toLocalFile();
    }
    else
        _fileName = path;
    _data->openFile(_fileName);
    loadStructure();
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

    emit sectionsChanged();
}

void FileDocument::addBlockSections(Sector * sector)
{
    for (Block * b : sector->childs())
    {
        AddressRange * r = b->range();
        r->setParent(this);
        _sections.append(r);

        Sector * s = b->as_sector();
        if (s) {
            addBlockSections(s);
        }
    }
}

void FileDocument::loadStructure()
{
    for (int i=0; i<_sections.size(); i++)
    {
        if (_sections[i]->isBlock())
        {
            _sections.removeAt(i);
            i--;
        }
    }

    _structure = read_structure(_fileName);
    if (_structure)
    {
        _structure->setDataSource(_data);
        addBlockSections(_structure);
    }

    emit sectionsChanged();
}

Sector* FileDocument::structure()
{
    return _structure;
}
