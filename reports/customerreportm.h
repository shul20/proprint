#ifndef CUSTOMERREOPRTM_H
#define CUSTOMERREOPRTM_H
#include "reportmodel.h"
#include <QStringList>
#include "utility/money.h"

class CustomerReportM : public ReportModel
{
    Q_OBJECT
private:
    static const QStringList header;


public:
    explicit CustomerReportM(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const;
    void getTotal(Money &total, Money &debt, Size &size);

public:
    static const QString queryStr;
    
signals:
    
public slots:
    
};

#endif // CUSTOMERREOPRTM_H
