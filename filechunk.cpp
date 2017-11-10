#include "filechunk.h"

#include "filedata.h"

FileChunk::FileChunk(const char * data, int size, int rowSize)
    : _data(data)
    , _size(size)
    , _rowSize(rowSize)
{
    _rowCount = qRound(size / rowSize + 0.5);
    _rows = new FileDataPtr[_rowCount];
}

FileChunk::~FileChunk()
{
    delete _rows;
}

std::vector<char> FileChunk::getData(int offset, int size) const
{
    Q_ASSERT(offset >= 0 && offset + size < _size);

    return std::vector<char>(_data + offset, _data + offset + size);
}

int FileChunk::size() const
{
    return _size;
}

FileDataPtr FileChunk::getRow(int number)
{
    Q_ASSERT(number < _rowCount);

    FileDataPtr ptr = _rows[number];
    if (ptr) return ptr;

    int size = _rowSize;
    int offset = number * _rowSize;
    if (offset + size > _size)
        size = _size - offset;

    return _rows[number] = std::make_shared<FileData>(_data + number * _rowSize, size);
}
