#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>

class FileModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int size READ size NOTIFY sizeChanged)
    Q_PROPERTY(int rows READ rows NOTIFY rowsChanged)
public:
    enum FileRoles {
        AddressRole = Qt::UserRole + 1,
        HexRole,
        TextRole
    };

    explicit FileModel(QObject *parent = 0);

    ~FileModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    int size() const;

    int rows() const;

signals:
    void sizeChanged(int);
    void rowsChanged(int);

public slots:
    void openFile(const QString &path);

private:
    class FileModelImpl;

    FileModelImpl * impl;
};

#endif // FILEMODEL_H
