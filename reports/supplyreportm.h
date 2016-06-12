#ifndef SUPPLYREPORTM_H
#define SUPPLYREPORTM_H
#include "reportmodel.h"
#include <QStringList>
#include "utility/money.h"


class SupplyReportM : public ReportModel
{
    Q_OBJECT
private:
    static const QStringList header;

public:
    explicit SupplyReportM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    void getTotal(Money &total, Money &debt);

    static const QString queryStr;
signals:
    
public slots:
    
};

#endif // SUPPLYREPORTM_H
