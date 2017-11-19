#ifndef FILEDOCUMENT_H
#define FILEDOCUMENT_H

#include "filemodel.h"
#include "addressrange.h"
#include "structuremodel.h"

class FileDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(int sectionCount READ sectionCount)
    Q_PROPERTY(FileModel* data READ data CONSTANT)
    Q_PROPERTY(StructureModel* structure READ structure CONSTANT)

public:
    FileDocument(QObject *parent = 0);
    ~FileDocument();

    Q_INVOKABLE void openFile(const QString &path);
    const QString & fileName() const;

    FileModel * data() const;

    int sectionCount() const;
    Q_INVOKABLE AddressRange* section(int index) const;
    Q_INVOKABLE void addSection(int begin, int end);

    StructureModel * structure() const;

private:
    QString _fileName;
    FileModel * _data;
    QList<AddressRange*> _sections;
    StructureModel * _structure;
};

#endif // FILEDOCUMENT_H
