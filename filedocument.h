#ifndef FILEDOCUMENT_H
#define FILEDOCUMENT_H

#include <QQmlListProperty>

#include "filemodel.h"
#include "addressrange.h"
#include "structure.h"
#include "structuremodel.h"

class FileDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(FileModel* data READ data CONSTANT)
    Q_PROPERTY(QStandardItemModel* structure READ structure CONSTANT)
    Q_PROPERTY(QQmlListProperty<AddressRange> sections READ sections CONSTANT)

public:
    FileDocument(QObject *parent = 0);
    ~FileDocument();

    Q_INVOKABLE void openFile(const QString &path);
    const QString & fileName() const;

    FileModel * data() const;

    QQmlListProperty<AddressRange> sections();

    Q_INVOKABLE void addSection(int begin, int end);

    QStandardItemModel * structure() const;

signals:
    void sectionsChanged();

private:
    QString _fileName;
    FileModel * _data;
    QList<AddressRange*> _sections;

    StructureModel * _structureModel;
};

#endif // FILEDOCUMENT_H
