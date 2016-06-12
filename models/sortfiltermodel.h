#ifndef SORTFILTERMODEL_H
#define SORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QSet>


class SortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

    typedef bool (*funcPtr)(QVariant, QVariant);

private:
    QVariant matchValue;

public:
    explicit SortFilterModel(QObject *parent = 0);
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const;
    
    QVariant getMatchValue() const;
    void setMatchValue(const QVariant &value);

    funcPtr compare;

    static bool equal(QVariant filter, QVariant dataFromModel);
    static bool contain(QVariant filter, QVariant dataFromModel);
    static bool paramFilter(QVariant filter, QVariant dataFromModel);

signals:
    
public slots:


};



#endif // SORTFILTERMODEL_H
