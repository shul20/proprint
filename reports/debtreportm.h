#ifndef DEBTREPORTM_H
#define DEBTREPORTM_H

#include "reportmodel.h"
#include <QStringList>
#include "utility/money.h"

class DebtReportM : public ReportModel
{
    Q_OBJECT
private:
    static const QString queryStr;
    static const QStringList header;


public:
    explicit DebtReportM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    Money getTotal();
    const QString &getQueryStr();
    
signals:
    
public slots:
    
};

#endif // DEBTREPORTM_H
