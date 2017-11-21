#ifndef BLOCK_H
#define BLOCK_H

#include "idatasource.h"

namespace structure {

    class Block
    {
    protected:
        IDataSource * _dataSource;
        int _offset; // Позиция в файле. Если < 0, позиция не известна
        QString _name;
        QString _description;
        bool _isValid;
        int _size;

    public:
        Block();

        virtual ~Block();

        void setDataSource(IDataSource * dataSource);

        void setOffset(int offset);
        int offset() const;
        int endOffset() const;

        const QString & name() const;
        void setName(const QString & name);
        void setDescription(const QString & description);

        virtual QString toString() const = 0;

        virtual bool isValid() const;

        int getSize() const;

    protected:
        virtual bool updateData() = 0;
    };

}

#endif // BLOCK_H
