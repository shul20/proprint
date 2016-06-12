#ifndef STOCKPROXY_H
#define STOCKPROXY_H

#include <QSortFilterProxyModel>
#include <QStringList>
#include "models/sortfiltermodel.h"

class StockProxy : public QSortFilterProxyModel
{
    Q_OBJECT
private:
    QStringList header;
    QString title;
    int type;

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
public:
    explicit StockProxy(QObject *parent, int type);
    const QString &getTitle() const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                                     int role = Qt::DisplayRole) const;
    
private slots:


signals:
    
public slots:

    
};

#endif // STOCKPROXY_H
