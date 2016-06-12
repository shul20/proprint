#ifndef EQUIPREPORTM_H
#define EQUIPREPORTM_H

#include <QSqlQuery>
#include <QStringList>
#include "utility/size.h"
#include "reportmodel.h"

class EquipReportM : public ReportModel
{
    Q_OBJECT
private:
    static const QString queryStr;
    static const QStringList header;


public:
    explicit EquipReportM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Size getTotalSize();
    
signals:
    
public slots:
    
};

#endif // EQUIPREPORTM_H
