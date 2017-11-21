#ifndef FILEDOCUMENT_H
#define FILEDOCUMENT_H

#include <QStandardItemModel>
#include <QQmlListProperty>

#include "filemodel.h"
#include "addressrange.h"
#include "structure.h"

class FileDocument : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString fileName READ fileName CONSTANT)
    Q_PROPERTY(FileModel* data READ data CONSTANT)
    Q_PROPERTY(QStandardItemModel* structure READ structure CONSTANT)
    Q_PROPERTY(QQmlListProperty<AddressRange> blocks READ blocks CONSTANT)
    Q_PROPERTY(QQmlListProperty<AddressRange> sections READ sections CONSTANT)

public:
    FileDocument(QObject *parent = 0);
    ~FileDocument();

    Q_INVOKABLE void openFile(const QString &path);
    const QString & fileName() const;

    FileModel * data() const;

    QQmlListProperty<AddressRange> sections();

    Q_INVOKABLE void addSection(int begin, int end);

    void addSectorToTree(structure::Sector & s, QStandardItem *parentItem);
    void buildStructure();
    QStandardItemModel * structure() const;
    QQmlListProperty<AddressRange> blocks();
    Q_INVOKABLE void selectBlock(QModelIndex index);

    void selectRange(AddressRange * range);

signals:
    void sectionsChanged();

private:
    QString _fileName;
    FileModel * _data;
    QList<AddressRange*> _sections;

    structure::Sector _structure;
    QStandardItemModel * _structureModel;
    QList<AddressRange*> _blocks;

    AddressRange* _selectedRange;
};

#endif // FILEDOCUMENT_H
