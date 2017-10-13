#ifndef FILEDATA_H
#define FILEDATA_H

class FileData
{
public:
    explicit FileData(std::shared_ptr<FileChunk> chunk);

    QString asText();
    QString asHex();

private:
    FileChunkPtr _chunk;
};

#endif // FILEDATA_H
