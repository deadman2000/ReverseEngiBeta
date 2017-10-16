#include "filemodel.h"

#include <QFile>

#include <unordered_map>

#include "filechunk.h"
#include "filedata.h"

class FileModel::FileModelImpl
{
    FileChunkPtr * _chunks;
    int _chunkCount;
    QFile * _file;
    int _fileSize;
    const int rowSize = 16;
    const int rowsInChunk = 128;

public:
    QString filePath;
    int rowCount;

    FileModelImpl()
        : _chunks(nullptr)
        , _chunkCount(0)
        , _file(nullptr)
        , _fileSize(0)
        , rowCount(0)
    {}

    void openFile(const QString &path)
    {
        if (_file){
            _file->close();
            delete _file;
            _file = nullptr;

            delete _chunks;
            _chunks = nullptr;
        }
        _file = new QFile(path);
        _file->open(QFile::ReadOnly);

        filePath = path;
        _fileSize = static_cast<int>(_file->size());
        rowCount = qRound(_fileSize / rowSize + 0.5);


        _chunkCount = qRound(rowCount / rowsInChunk + 0.5);
        _chunks = new FileChunkPtr[_chunkCount];

        qDebug() << "FileSize: " << _fileSize;
        qDebug() << "Rows: " << rowCount;
        qDebug() << "Chunks: " << _chunkCount;
    }

    FileChunkPtr getChunk(int number)
    {
        Q_ASSERT(number < _chunkCount);

        FileChunkPtr ptr = _chunks[number];
        if (ptr) return ptr;

        int size = rowsInChunk * rowSize;
        int offset = number * size;
        if (offset + size > _fileSize)
            size = _fileSize - offset;

        _file->seek(offset);
        char* data = new char[size];
        int bytes = static_cast<int>(_file->read(data, size));
        Q_ASSERT(bytes == size);

        return _chunks[number] = std::make_shared<FileChunk>(data, size, rowSize);
    }

    FileDataPtr getRow(int row)
    {
        int chunkNumber = row / rowsInChunk;
        return getChunk(chunkNumber)->getRow(row - chunkNumber * rowsInChunk);
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
    beginResetModel();
    impl->openFile(path);
    endResetModel();
}
