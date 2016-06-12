#ifndef WOFFVIEW_H
#define WOFFVIEW_H

#include <QWidget>
#include <QDate>
#include <QMenu>

namespace Ui {
class WoffView;
}

class WoffView : public QWidget
{
    Q_OBJECT
    
public:
    explicit WoffView(QWidget *parent = 0);
    ~WoffView();
    
private slots:
    void on_stock_currentIndexChanged(int index);

    void on_dateFrom_dateChanged(const QDate &date);

    void on_dateTill_dateChanged(const QDate &date);

    void on_table_customContextMenuRequested(const QPoint &pos);

private:
    Ui::WoffView *ui;
    QMenu *menu;
    QDate dateFrom, dateTill;

    void appendMenu();

private slots:
    void refresh();
};

#endif // WOFFVIEW_H
