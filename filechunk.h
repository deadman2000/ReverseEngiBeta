#ifndef FILECHUNK_H
#define FILECHUNK_H

class FileData;

class FileChunk
{
public:
    FileChunk();

    FileDataPtr getData(int offset, int size);
private:
    int _offset;
    int _size;
};

#endif // FILECHUNK_H
