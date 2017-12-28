#include "sector.h"

#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>

Sector::Sector()
{
}

Sector::~Sector()
{
    qDeleteAll(_childs);
}

int Sector::typeID() const
{
    return 3;
}

QString Sector::typeName() const
{
    return "sector";
}

void Sector::setDataSource(IDataSource *dataSource)
{
    Block::setDataSource(dataSource);
    for (Block* block : _childs)
    {
        block->setDataSource(dataSource);
    }
}

QString Sector::toString() const
{
    return QString();
}

void Sector::append(Block *block)
{
    _childs.append(block);
    makeChild(block);
}

void Sector::insert(int index, Block *block)
{
    _childs.insert(index, block);
    makeChild(block);
}

void Sector::replace(int index, Block *block)
{
    Q_ASSERT(index>= 0 && index < _childs.size());

    delete _childs[index];
    _childs[index] = block;
    makeChild(block);
}

void Sector::remove(Block *block)
{
    _childs.removeOne(block);
    updateData();
}

void Sector::makeChild(Block *block)
{
    block->setParent(this);
    block->setDataSource(_dataSource);

    updateData();

    /*if (_isValid && _offset >= 0)
    {
        block->setOffset(_offset + _size);
        if (block->isValid())
            _size += block->getSize();
        else
            _isValid = false;
    }

    if (_isValid && _parent)
        _parent->updateData();*/ // Пересчитываем размер нижнего блока
}

const QList<Block *> &Sector::childs() const
{
    return _childs;
}

bool Sector::updateData()
{
    int s = 0;
    for (Block* block : _childs)
    {
        block->setOffset(_offset + s);
        if (block->isValid())
            s += block->size();
        else
            return false;
    }
    setSize(s);
    return true;
}

void Sector::readAttr(const QJsonObject &json)
{
    if (json.contains("childs"))
    {
        qDeleteAll(_childs);
        _childs.clear();

        const QJsonArray & arr = json["childs"].toArray();
        foreach (const QJsonValue & value, arr) {
            QJsonObject obj = value.toObject();
            int type = obj["type"].toInt();
            Block * block = Block::create(type);
            block->acceptJSON(obj);
            append(block);
        }
    }
}

bool Sector::load(const QString &fileName)
{
    qDebug() << "Load" << fileName;
    QFile file(fileName);

    if (!file.exists()){
        qWarning() << QString("File %1 not found").arg(fileName);
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument doc(QJsonDocument::fromJson(file.readAll()));
    acceptJSON(doc.object());

    return true;
}

bool Sector::save(const QString &fileName) const
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject root;
    save(root);

    QJsonDocument doc(root);
    file.write(doc.toJson());

    return true;
}

void Sector::save(QJsonObject &json) const
{
    Block::save(json);

    QJsonArray childs;
    for (Block* block : _childs)
    {
        QJsonObject obj;
        block->save(obj);
        childs.push_back(obj);
    }
    json["childs"] = childs;
}
