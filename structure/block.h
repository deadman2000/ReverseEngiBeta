#ifndef BLOCK_H
#define BLOCK_H

#include <QJsonObject>

#include "idatasource.h"

namespace structure {
    class Sector;

    class Block
    {
    protected:
        IDataSource * _dataSource;
        Sector * _parent;
        int _offset; // Позиция в файле. Если < 0, позиция не известна
        bool _isValid;
        int _size;

        QString _name;
        QString _description;

    public:
        Block();
        virtual ~Block();

        virtual int typeID() const = 0;

        virtual void setDataSource(IDataSource * dataSource);

        void setParent(Sector * parent);
        Sector * parent() const;

        void setOffset(int offset);
        int offset() const;
        int endOffset() const;

        const QString & name() const;
        void setName(const QString & name);
        void setDescription(const QString & description);

        virtual QString toString() const = 0;

        virtual bool isValid() const;

        void acceptJSON(const QJsonObject & json); // Применяет атрибуты и структу из JSON
        void toJSON(QJsonObject & json);           // Сохраняет атрибуты блока в JSON
        virtual void save(QJsonObject &json);              // Сохраняет полную структуру блока в JSON

        int size() const;
        void setSize(int value);

        static Block* create(int typeId);

    protected:
        virtual bool updateData() = 0;
        virtual void readAttr(const QJsonObject & json);         // Сохраняет доп. атрибуты блока в JSON
        virtual void writeAttr(QJsonObject & json);         // Сохраняет доп. атрибуты блока в JSON
    };

}

#endif // BLOCK_H
