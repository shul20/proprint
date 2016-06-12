#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QWidget>
#include <QDate>
#include <QMenu>
#include <QSortFilterProxyModel>
#include "models/stock/materialmodel.h"
#include "dialogs/stock/materialdialog.h"

namespace Ui {
class MaterialWidget;
}

class MaterialWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MaterialWidget(QWidget *parent = 0);
    ~MaterialWidget();
    QAbstractItemModel *getModel();
    int currentStock();
    
private slots:
    void on_stock_currentIndexChanged(int index);

    void on_startDate_dateChanged(const QDate &date);

    void on_endDate_dateChanged(const QDate &date);

    void editMaterial();
    void editMaterial(QModelIndex index);
    void delMaterial();
    void addPayment();

    void on_table_customContextMenuRequested(const QPoint &pos);

private:
    MaterialModel *model;
    Ui::MaterialWidget *ui;
    QDate startDate, endDate;
    QMenu *menu;

    void appendMenu();
};

#endif // MATERIALWIDGET_H
