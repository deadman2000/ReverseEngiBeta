#ifndef FILEDOCUMENT_H
#define FILEDOCUMENT_H

#include <QQmlListProperty>
#include <QUrl>

#include "filemodel.h"
#include "addressrange.h"
#include "structure.h"
#include "structuremodel.h"

class FileDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(FileModel* data READ data CONSTANT)
    Q_PROPERTY(QQmlListProperty<AddressRange> sections READ sections CONSTANT)

public:
    FileDocument(QObject *parent = 0);
    ~FileDocument();

    const QString & fileName() const;

    FileModel * data() const;

    QQmlListProperty<AddressRange> sections();

    Sector * structure();

public slots:
    void openFile(const QString &path);

    void addSection(int begin, int end);

    void loadStructure(const QUrl & url);
    void loadStructure(const QString & path);

signals:
    void sectionsChanged();

private:
    QString _fileName;
    FileModel * _data;
    QList<AddressRange*> _sections;

    Sector _structure;
};

#endif // FILEDOCUMENT_H
