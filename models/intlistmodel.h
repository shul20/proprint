#ifndef INTLISTMODEL_H
#define INTLISTMODEL_H

#include <QAbstractListModel>

class IntListModel : public QAbstractListModel
{
    Q_OBJECT
private:
    QList<int> list;

public:
    explicit IntListModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    void appendInt(int value);
    void clear();

signals:
    
public slots:
    
};

#endif // INTLISTMODEL_H
