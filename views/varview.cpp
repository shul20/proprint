#include "varview.h"
#include "ui_varview.h"
#include "models/varmodel.h"
#include "models/modelmanager.h"

VarView::VarView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VarView)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromUtf8("Переменные программы"));

    VarModel *varModel = ModelManager::getInstance()->getVarModel();
    varModel->setIsRefresh(false);

    ui->table->setModel(varModel);

    QMenu *menu = ui->table->getMenu();
    menu->removeAction(ui->table->actionAdd);
    menu->removeAction(ui->table->actionEdit);
    menu->removeAction(ui->table->actionDel);
}

VarView::~VarView()
{
    VarModel *varModel = (VarModel *) ui->table->model();
    varModel->setIsRefresh(true);

    delete ui;
}
