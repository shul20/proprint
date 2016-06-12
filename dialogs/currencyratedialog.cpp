#include "currencyratedialog.h"
#include "ui_currencyratedialog.h"
#include "proprintlib.h"
#include "models/varmodel.h"
#include "models/modelmanager.h"

CurrencyRateDialog::CurrencyRateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrencyRateDialog)
{
    ui->setupUi(this);

    VarModel *var = ModelManager::getInstance()->getVarModel();
    Money rate(var->getVar(Var::RATE));
    ui->info->setText(QString::fromUtf8("1 USD = %1 грн").arg(rate.toString()));
    ui->rate->setMoney(rate);
}

CurrencyRateDialog::~CurrencyRateDialog()
{
    delete ui;
}

void CurrencyRateDialog::on_saveButton_clicked()
{
    Money rate = ui->rate->getMoney();
    if (rate == 0) {
        PPLib::fillFormMessage();
    }

    VarModel *var = ModelManager::getInstance()->getVarModel();
    var->setVar(Var::RATE, rate.cents);

    close();
}

void CurrencyRateDialog::on_exitButton_clicked()
{
    close();
}

void CurrencyRateDialog::on_rate_textChanged(const QString &arg1)
{
    ui->info->setText(QString::fromUtf8("1 USD = %1 грн").arg(arg1));
}
