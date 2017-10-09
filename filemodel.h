#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QAbstractListModel>
#include <QFile>

class FileModel : public QAbstractListModel
{
Q_OBJECT

    public:
        enum FileRoles {
            AddressRole = Qt::UserRole + 1,
            HexRole,
            TextRole
        };

        explicit FileModel(QObject *parent = 0);

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        virtual QHash<int, QByteArray> roleNames() const override;

    public slots:
        void setFilePath(const QString &path);

    //protected:
       //bool canFetchMore(const QModelIndex &parent) const override;
       //void fetchMore(const QModelIndex &parent) override;

    private:
        QString _filePath;
        quint64 _fileSize;
        int _rowCount;
        QFile _file;
};

#endif // FILEMODEL_H
