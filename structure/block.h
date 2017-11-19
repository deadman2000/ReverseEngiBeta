#ifndef BLOCK_H
#define BLOCK_H

#include "idatasource.h"

namespace structure {

    class Block
    {
    protected:
        IDataSource * _dataSource;
        int _offset;
        QString _name;
        QString _description;

    public:
        Block();

        virtual ~Block();

        void setDataSource(IDataSource * dataSource);
        void setOffset(int offset);
        void setName(const QString & name);
        void setDescription(const QString & description);

        virtual QString toString() const = 0;

        virtual bool isValid() const;

        virtual int getSize() const = 0;

    protected:
        virtual void updateData() = 0;
    };

}

#endif // BLOCK_H
