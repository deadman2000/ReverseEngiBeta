#ifndef STDAFX_H
#define STDAFX_H

#include <memory>

#include <QObject>
#include <QString>
#include <QDebug>

class FileChunk;
typedef std::shared_ptr<FileChunk> FileChunkPtr;

class FileData;
typedef std::shared_ptr<FileData> FileDataPtr;

#endif // STDAFX_H
