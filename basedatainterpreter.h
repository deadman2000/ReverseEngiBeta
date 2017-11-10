#ifndef IDATAINTERPRETER_H
#define IDATAINTERPRETER_H

#include "idatasource.h"

#include <QObject>
#include <QString>

class BaseDataInterpreter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
public:
    BaseDataInterpreter();

    virtual ~BaseDataInterpreter();

    virtual QString name() const = 0;

    virtual QString toString(IDataSource * dataSource, int offset) const = 0;

public slots:
    QString value() const;
};

#endif // IDATAINTERPRETER_H
