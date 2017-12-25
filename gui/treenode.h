#pragma once

#include <QObject>

class TreeModel;

class TreeNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(TreeModel * children READ children NOTIFY childrenChanged)
public:
    explicit TreeNode(TreeModel * parent = nullptr);
    ~TreeNode();

    QString text() const;
    void setText(const QString & text);

    QString icon() const;
    void setIcon(const QString & icon);

    TreeModel * children() const;
    void appendNode(TreeNode * obj);
    TreeNode * createChild();

    TreeModel * model() const;
    void setModel(TreeModel* model);

signals:
    void textChanged(QString);
    void iconChanged(QString);
    void childrenChanged(TreeModel *);

public slots:
    void moveBefore(TreeNode * target);
    void moveAfter(TreeNode * target);

private:
    TreeModel * _model;
    QString _text;
    QString _icon;
    TreeModel * _children;
};
