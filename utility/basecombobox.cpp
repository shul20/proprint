#include "basecombobox.h"

BaseComboBox::BaseComboBox(QWidget *parent) :
    QComboBox(parent), isBlock(false){}

BaseModel *BaseComboBox::getModel() const{
    return model;
}

void BaseComboBox::setParam(BaseModel *model, int column)
{
}

void BaseComboBox::setModel(QSortFilterProxyModel *model, int column){
    BaseModel *parent = static_cast<BaseModel *>(model->sourceModel());
    this->model = (BaseModel *)model;
    this->column = column;
    updateBox();
    connect(parent, SIGNAL(modelUpdate()), this, SLOT(updateBox()));
}

void BaseComboBox::setModel(BaseModel *model, int column){
    this->column = column;
    this->model = model;
    connect(model, SIGNAL(modelUpdate()), this, SLOT(updateBox()));
}

int BaseComboBox::setCurrent(int index){
    setCurrentIndex(index);
    return getId(index);
}

void BaseComboBox::setById(int id){
    setCurrentIndex(getIndexById(id));
}

int BaseComboBox::getCurrentId(){
    int index = currentIndex();
    return itemData(index).toInt();
}

int BaseComboBox::getId(int index){
    return itemData(index).toInt();
}

int BaseComboBox::getIndexById(int id) {
    return findData(id);
}

void BaseComboBox::block(bool isBlock){
    this->isBlock = isBlock;
}

void BaseComboBox::updateBox(){
    int rows = model->rowCount();

    blockSignals(true);
    clear();
    blockSignals(false);

    for (int i = 0; i < rows; i++){
        const QVariant name = model->sibling(i, column, QModelIndex()).data();
        const QVariant id = model->sibling(i, 0, QModelIndex()).data(Qt::UserRole);
        insertItem(i, name.toString(), id);
    }
}


void BaseComboBox::mousePressEvent(QMouseEvent *event){
    if (isBlock) {
        event->accept();
        return;
    }
    QComboBox::mousePressEvent(event);
}

void BaseComboBox::keyPressEvent(QKeyEvent *e){
    if (isBlock) {
        e->accept();
        return;
    }
    QComboBox::keyPressEvent(e);
}
