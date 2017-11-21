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

        void append(Block * block);
        const QList<Block *> & childs() const;

    protected:
        virtual bool updateData() override;
    private:
        QList<Block *> _childs;
    };
}

#endif // SECTOR_H
