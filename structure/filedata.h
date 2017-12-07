#ifndef FILEDATA_H
#define FILEDATA_H

class FileData
{
    const char * _data;
    int _size;

public:
    FileData(const char * data, int size);

    QString asText();
    QString asHex();
};

#endif // FILEDATA_H
