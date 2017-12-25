#ifndef INTERPRETERMODEL_H
#define INTERPRETERMODEL_H

#include <QAbstractListModel>
#include "basedatainterpreter.h"
#include "filemodel.h"

class InterpreterModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(FileModel * model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(int offset READ offset WRITE setOffset NOTIFY offsetChanged)

    enum FileRoles {
        NameRole = Qt::UserRole + 1,
        ValueRole
    };
public:
    InterpreterModel();

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    FileModel * model() const;
    void setModel(FileModel * value);

    int offset() const;
    void setOffset(int value);

signals:
    void modelChanged(FileModel *);
    void offsetChanged(int);

private:
    QList<BaseDataInterpreter*> _interpreters;
    FileModel * _model;
    int _offset;
};

#endif // INTERPRETERMODEL_H
