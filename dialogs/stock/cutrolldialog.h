#ifndef CUTROLLDIALOG_H
#define CUTROLLDIALOG_H

#include <QDialog>
#include "entity/stock/stock.h"
#include "models/stock/stockmodel.h"
#include "models/intlistmodel.h"
#include "models/sortfiltermodel.h"

namespace Ui {
class CutRollDialog;
}

class CutRollDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CutRollDialog(QAbstractItemModel *model, Stock *stock);
    ~CutRollDialog();
    
private slots:
    void on_cutButton_clicked();

    void on_saveButton_clicked();

    void on_closeButton_clicked();

    void on_cutWidth_valueChanged(int value);

    void on_resetButton_clicked();

    void on_rolls_doubleClicked(const QModelIndex &index);

private:
    Ui::CutRollDialog *ui;
    Stock *stock;
    SortFilterModel *widthModel;
    int width, lengthM, cutWidth, rollAmount;
    IntListModel *cutWidthModel;

    void entity2Form(Stock *stock);
    Stock *findStock(int width);
};

#endif // CUTROLLDIALOG_H
