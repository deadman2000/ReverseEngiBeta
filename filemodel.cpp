#include "filemodel.h"

#include <QDebug>

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
    , _fileSize(0)
    , _rowCount(0)
{
}

QHash<int, QByteArray> FileModel::roleNames() const {
    qDebug() << "roleNames";
    QHash<int, QByteArray> roles;
    roles[AddressRole] = "address";
    roles[HexRole] = "hex";
    roles[TextRole] = "text";
    return roles;
}

int FileModel::rowCount(const QModelIndex &) const
{
    qDebug() << "rowCount =" << _rowCount;
    return _rowCount;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _rowCount || index.row() < 0)
        return QVariant();

    qDebug() << "data(" << index.row() << ", " << role <<")";

    if (role == AddressRole)
        return index.row() * 16;
    else if (role == HexRole)
        return QString("00 99 00 0A BC DE F0 00  01 02 03 04 05 06 07 08");
    else if (role == TextRole)
        return QString("........");
    return QVariant();
}

void FileModel::setFilePath(const QString &path)
{
    _file.close();
    _file.setFileName(path);
    _file.open(QFile::ReadOnly);
    qDebug() << "Set file:" << path;
    qDebug() << "Size:" << _file.size();

    beginResetModel();
    _filePath = path;
    _fileSize = _file.size();
    _rowCount = _fileSize / 16;
    endResetModel();
}
