#include "filedocument.h"

#include "structure.h"

FileDocument::FileDocument(QObject *parent)
    : QObject(parent)
{
    _data = new FileModel(this);
    _structure = new StructureModel(this);

    // TODO Потом убрать
    {
        auto * b = new structure::Number();
        _structure->append(b);
    }
}

FileDocument::~FileDocument()
{
    _data->deleteLater();
    _structure->deleteLater();
}

void FileDocument::openFile(const QString &path)
{
    _data->openFile(path);
}

const QString & FileDocument::fileName() const
{
    return _fileName;
}

FileModel *FileDocument::data() const
{
    return _data;
}

int FileDocument::sectionCount() const
{
    return _sections.size();
}

AddressRange * FileDocument::section(int index) const
{
    return _sections.at(index);
}

void FileDocument::addSection(int begin, int end)
{
    _sections.append(new AddressRange(this, begin, end, QColor(0, 255, 0, 25), QColor(0, 255, 0), 2));
}

StructureModel *FileDocument::structure() const
{
    return _structure;
}
