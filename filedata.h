#ifndef FILEDATA_H
#define FILEDATA_H

class FileData
{
    const char * _data;
    int _size;
    QString _text;
    QString _hex;

public:
    FileData(const char * data, int size);

    QString asText();
    QString asHex();
};

#endif // FILEDATA_H
