#include "interpretermodel.h"

#include "iengine.h"

InterpreterModel::InterpreterModel()
    : QAbstractListModel()
    , _model(nullptr)
{
    _interpreters = engine().interpreters();
}

QHash<int, QByteArray> InterpreterModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    return roles;
}

FileModel *InterpreterModel::model() const
{
    return _model;
}

void InterpreterModel::setModel(FileModel *value)
{
    beginResetModel();
    _model = value;
    emit modelChanged(value);
    endResetModel();
}

int InterpreterModel::offset() const
{
    return _offset;
}

void InterpreterModel::setOffset(int value)
{
    beginResetModel();
    _offset = value;
    emit offsetChanged(value);
    endResetModel();
}

int InterpreterModel::rowCount(const QModelIndex &) const
{
    return _interpreters.size();
}

QVariant InterpreterModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= _interpreters.size() || index.row() < 0)
        return QVariant();

    if (role == NameRole)
        return _interpreters.at(index.row())->name();

    if (role == ValueRole){
        if (!_model) return QVariant();
        return _interpreters.at(index.row())->toString(_model, _offset);
    }

    return QVariant();
}
