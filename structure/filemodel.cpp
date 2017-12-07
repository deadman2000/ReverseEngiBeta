#include "filemodel.h"

#include <QFile>
#include <QUrl>
#include <QGuiApplication>
#include <QClipboard>

#include <unordered_map>

#include "filechunk.h"
#include "filedata.h"

class FileModel::FileModelImpl
{
public:
    FileChunkPtr * chunks;
    int chunkCount;
    QFile * file;
    int fileSize;
    const int rowSize = 16;
    const int rowsInChunk = 128;
    const int chunkSize = rowSize * rowsInChunk;

    QString filePath;
    int rowCount;

    FileModelImpl()
        : chunks(nullptr)
        , chunkCount(0)
        , file(nullptr)
        , fileSize(0)
        , rowCount(0)
    {}

    ~FileModelImpl()
    {
        delete[] chunks;
    }

    void openFile(const QString &path)
    {
        if (file){
            file->close();
            delete file;
            file = nullptr;

            delete chunks;
            chunks = nullptr;
        }
        file = new QFile(path);
        file->open(QFile::ReadOnly);

        filePath = path;
        fileSize = static_cast<int>(file->size());
        rowCount = qRound(fileSize / double(rowSize) + 0.49999999);

        chunkCount = qRound(rowCount / rowsInChunk + 0.5);
        chunks = new FileChunkPtr[chunkCount];

        qDebug() << "FileSize: " << fileSize;
        qDebug() << "Rows: " << rowCount;
        qDebug() << "Chunks: " << chunkCount;
    }

    FileChunkPtr getChunk(int number)
    {
        Q_ASSERT(number < chunkCount);

        FileChunkPtr ptr = chunks[number];
        if (ptr) return ptr;

        int size = chunkSize;
        int offset = number * size;
        if (offset + size > fileSize)
            size = fileSize - offset;

        file->seek(offset);
        char* data = new char[size];
        int bytes = static_cast<int>(file->read(data, size));
        Q_ASSERT(bytes == size);

        return chunks[number] = std::make_shared<FileChunk>(data, size, rowSize);
    }

    FileDataPtr getRow(int row)
    {
        int chunkNumber = row / rowsInChunk;
        return getChunk(chunkNumber)->getRow(row - chunkNumber * rowsInChunk);
    }

    QByteArray getData(int offset, int size)
    {
        QByteArray arr;
        arr.reserve(size);

        int dataEnd = offset + size ;
        int chunkBegin = offset / chunkSize;
        int chunkEnd = (offset + size) / chunkSize;

        for (int i = chunkBegin; i<=chunkEnd; ++i)
        {
            auto chunk = getChunk(i);
            int chunkOffset = i * chunkSize;
            int chunkOffsetEnd = chunkOffset + std::min(chunkSize, chunk->size());

            int from = std::max(0, offset - chunkOffset);
            int to   = std::min(chunkOffsetEnd, dataEnd) - chunkOffset;

            if (to == from) break;

            auto subData = chunk->getData(from, to - from);
            arr.append(subData);
        }

        return arr;
    }
};

FileModel::FileModel(QObject *parent)
    : QAbstractListModel(parent)
    , impl(new FileModelImpl)
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
    roles[TextRole] = "fileText";
    return roles;
}

int FileModel::size() const
{
    return impl->fileSize;
}

int FileModel::rows() const
{
    return impl->rowCount;
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

    if (role == AddressRole){
        int offset = index.row() * 16;
        return QString("%1 %2").arg(offset >> 16, 4, 16, QLatin1Char('0')).arg(offset & 0xFFFF, 4, 16, QLatin1Char('0'));
    }

    if (role == HexRole)
        return impl->getRow(index.row())->asHex();

    if (role == TextRole)
        return impl->getRow(index.row())->asText();
    return QVariant();
}

void FileModel::openFile(const QString &path)
{
    QString filePath = path;

    QUrl url(path);
    if (url.isValid() && url.isLocalFile())
        filePath = url.toLocalFile();

    beginResetModel();
    impl->openFile(filePath);
    endResetModel();

    emit sizeChanged(size());
    emit rowsChanged(rows());
}


QByteArray FileModel::getData(int offset, int size)
{
    return impl->getData(offset, size);
}

void FileModel::copyToClipboard(int begin, int end)
{
    auto data = getData(begin, end - begin + 1);
    QGuiApplication::clipboard()->setText(QString(data));
}
