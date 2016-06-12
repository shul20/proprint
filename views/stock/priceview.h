#ifndef PRICEVIEW_H
#define PRICEVIEW_H

#include <QWidget>
#include "dialogs/stock/pricedialog.h"
#include "models/sortfiltermodel.h"

namespace Ui {
class PriceView;
}

class PriceView : public QWidget
{
    Q_OBJECT
    
public:
    explicit PriceView(QWidget *parent = 0);
    ~PriceView();
    
private:
    Ui::PriceView *ui;
    PriceDialog *dialog;
    SortFilterModel *priceF, *findF;

private slots:
    void addPrice();
    void editPrice();
    void editPrice(QModelIndex index);
    void on_process_currentIndexChanged(int index);
    void on_company_textChanged(const QString &arg1);
};

#endif // PRICEVIEW_H
