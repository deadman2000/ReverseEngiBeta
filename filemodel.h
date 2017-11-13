#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>

#include "idatasource.h"
#include "addressrange.h"

class FileModel : public QAbstractListModel, public IDataSource
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(int rows READ rows NOTIFY rowsChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)
    Q_PROPERTY(int sectionCount READ sectionCount)

    enum FileRoles {
        AddressRole = Qt::UserRole + 1,
        HexRole,
        TextRole
    };
public:
    explicit FileModel(QObject *parent = 0);

    ~FileModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int size() const;
    int rows() const;

    QString & fileName() const;

    QByteArray getData(int offset, int size) override;

    int sectionCount() const;
    Q_INVOKABLE AddressRange* section(int index) const;
    Q_INVOKABLE void addSection(int begin, int end);

    Q_INVOKABLE void copyToClipboard(int begin, int end);

signals:
    void sizeChanged(int);
    void rowsChanged(int);
    void fileNameChanged(QString);

public slots:
    void openFile(const QString &path);

private:
    class FileModelImpl;

    FileModelImpl * impl;

    QList<AddressRange*> _sections;
};

#endif // FILEMODEL_H
