#ifndef SECTOR_H
#define SECTOR_H

#include "block.h"

namespace structure {

    class Sector : public Block
    {
    public:
        Sector();
        virtual ~Sector() override;

        // Block interface
    public:
        virtual QString toString() const override;
        virtual int getSize() const override;

        int count() const;
        void append(Block * block);

    protected:
        virtual void updateData() override;
    private:
        QList<Block *> _childs;
    };
}

#endif // SECTOR_H
