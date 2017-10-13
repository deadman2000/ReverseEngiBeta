#ifndef STDAFX_H
#define STDAFX_H

#include <memory>
#include <QString>

class FileChunk;
typedef std::shared_ptr<FileChunk> FileChunkPtr;

class FileData;
typedef std::shared_ptr<FileData> FileDataPtr;

#endif // STDAFX_H
