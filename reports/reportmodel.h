#ifndef REPORTMODEL_H
#define REPORTMODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QStringList>

class ReportModel : public QSqlQueryModel
{
    Q_OBJECT
protected:
    QSqlQuery query;
    const QStringList *header;

private:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    int columnCount(const QModelIndex &parent) const;

public:
    explicit ReportModel(QObject *parent = 0);
    QSqlQuery &getQuery();
    
signals:
    
public slots:
    
};

#endif // REPORTMODEL_H
