#ifndef EQUIPVIEW_H
#define EQUIPVIEW_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class EquipView;
}

class EquipView : public QWidget
{
    Q_OBJECT
    
public:
    explicit EquipView(QWidget *parent = 0);
    ~EquipView();

private:
    Ui::EquipView *ui;



private slots:
    void addEquip();
    void editEquip();
    void editEquip(QModelIndex index);
};

#endif // EQUIPVIEW_H
