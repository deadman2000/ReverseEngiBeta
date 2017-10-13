#include "filemodel.h"

#include <QDebug>
#include <QFile>

#include <unordered_map>

#include "filechunk.h"
#include "filedata.h"

#define CHUNK_SIZE 2048

class FileModel::FileModelImpl
{
public:
    QString filePath;
    int fileSize;
    int rowCount;
    QFile * file;
    std::unordered_map<int, FileChunkPtr> chunks;

    FileModelImpl()
        : fileSize(0)
        , rowCount(0)
        , file(nullptr)
    {}

    void openFile(const QString &path)
    {
        if (file){
            file->close();
            delete file;
            file = nullptr;
        }
        file = new QFile(path);
        file->open(QFile::ReadOnly);

        filePath = path;
        fileSize = static_cast<int>(file->size());
        rowCount = fileSize / 16;
    }

    FileChunkPtr getChunk(int number)
    {
        return FileChunkPtr();
    }

    FileDataPtr getData(int offset, int size)
    {
        int chunkFrom = offset / CHUNK_SIZE;
        int chunkTo = static_cast<int>(static_cast<double>(offset + size) / CHUNK_SIZE + 0.5);
        qDebug() << chunkFrom << " - " << chunkTo;

        auto chunk = getChunk(chunkFrom);

        return chunk->getData(offset, size);
    }
};

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
    , impl(new FileModelImpl())
{
}

FileModel::~FileModel()
{
    delete impl;
}

QHash<int, QByteArray> FileModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AddressRole] = "address";
    roles[HexRole] = "hex";
    roles[TextRole] = "text";
    return roles;
}

int FileModel::rowCount(const QModelIndex &) const
{
    return impl->rowCount;
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= impl->rowCount || index.row() < 0)
        return QVariant();

    int offset = index.row() * 16;
    if (role == AddressRole){
        return QString("%1 %2").arg(offset >> 16, 4, 16, QLatin1Char('0')).arg(offset & 0xFFFF, 4, 16, QLatin1Char('0'));
    } else if (role == HexRole) {
        return impl->getData(offset, 16)->asHex();
    } else if (role == TextRole) {
        return impl->getData(offset, 16)->asText();
    }
    return QVariant();
}

void FileModel::openFile(const QString &path)
{
    beginResetModel();
    impl->openFile(path);
    endResetModel();
}
