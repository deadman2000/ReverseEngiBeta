#pragma once

#include <QAbstractListModel>

class TreeNode : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(TreeNode* parentNode READ parentNode)

    enum TreeRoles {
        NodeRole = Qt::UserRole + 1
    };
public:
    explicit TreeNode(TreeNode * parent = nullptr);
    virtual ~TreeNode();

    QString text() const;
    void setText(const QString & text);

    QString icon() const;
    void setIcon(const QString & icon);

    TreeNode * parentNode() const;

    // Childs
    int count() const;
    virtual QHash<int, QByteArray> roleNames() const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    int indexOf(TreeNode * node) const;

signals:
    void textChanged(QString);
    void iconChanged(QString);
    void countChanged(int);

public slots:
    virtual void update();
    virtual bool canAppend(TreeNode * node) const;
    virtual void insert(int index, TreeNode * node);
    virtual void remove();
    virtual void removeChild(TreeNode * node);
    virtual void append(TreeNode * node);

    void moveBefore(TreeNode * target);
    void moveAfter(TreeNode * target);

protected:
    QString _text;
    QString _icon;

    QList<TreeNode*> _nodes;
};
