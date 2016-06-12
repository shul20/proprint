#ifndef UNIQUEFILTER_H
#define UNIQUEFILTER_H

#include <QSortFilterProxyModel>
#include <QSet>

class UniqueFilter : public QSortFilterProxyModel
{
    Q_OBJECT
private:
    mutable QSet<QString> cache;

public:
    explicit UniqueFilter(QObject *parent = 0);
    void setSourceModel(QAbstractItemModel *sourceModel);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
signals:
    
public slots:
    
private slots:
    void clearCache() { cache.clear(); }

};

#endif // UNIQUEFILTER_H
