#ifndef STRUCTUREMODEL_H
#define STRUCTUREMODEL_H

#include <QObject>

class StructureModel : public QObject
{
    Q_OBJECT
public:
    explicit StructureModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // STRUCTUREMODEL_H