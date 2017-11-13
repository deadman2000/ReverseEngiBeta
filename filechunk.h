#ifndef FILECHUNK_H
#define FILECHUNK_H

#include <QByteArray>

class FileData;

class FileChunk
{
    const char * _data;
    int _size;
    int _rowSize;
    FileDataPtr * _rows;
    int _rowCount;

public:
    FileChunk(const char * data, int size, int rowSize);
    ~FileChunk();

    QByteArray getData(int offset, int size) const;

    int size() const;

    FileDataPtr getRow(int number);
};

#endif // FILECHUNK_H
