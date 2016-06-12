#ifndef BASECOMBOBOX_H
#define BASECOMBOBOX_H

#include <QComboBox>
#include <QAbstractItemView>
#include <QMouseEvent>
#include <QSortFilterProxyModel>

#include "models/basemodel.h"

class BaseComboBox : public QComboBox
{
    Q_OBJECT
private:
    BaseModel *model;
    int column;
    bool isBlock;

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);


public:
    explicit BaseComboBox(QWidget *parent);
    BaseModel *getModel() const;
    void setParam(BaseModel *model, int column);
    void setModel(QSortFilterProxyModel *model, int column);
    void setModel(BaseModel *model, int column);
    int setCurrent(int index);
    void setById(int id);
    int getCurrentId();
    int getId(int index);
    int getIndexById(int id);
    void block(bool isBlock);
    
signals:
    
public slots:
    void updateBox();
};

#endif // BASECOMBOBOX_H
