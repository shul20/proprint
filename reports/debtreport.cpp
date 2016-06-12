#include "debtreport.h"
#include "ui_debtreport.h"
#include "widgets/entitytable.h"
#include <QDebug>
#include "utility/money.h"
#include "widgets/popupwindow.h"
#include <QTimer>
#include "entity/company.h"

DebtReport::DebtReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebtReport)
{
    ui->setupUi(this);

    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");

    model = new DebtReportM(this);
    ui->table->setModel(model);
    popMessage = new PopUpWindow(this, QString::fromUtf8("создание отчета"));

    ui->type->addItem(QString::fromUtf8("Заказчики"));
    ui->type->addItem(QString::fromUtf8("Поставщики/Подрядчики"));
}

DebtReport::~DebtReport()
{
    delete ui;
}

void DebtReport::on_startButton_clicked()
{
    popMessage->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTimer::singleShot(100, this, SLOT(execute()));
}

void DebtReport::execute()
{
    QSqlQuery &query = model->getQuery();
    int index = ui->type->currentIndex();
    QString str;

    if (index == 0) {
        str = model->getQueryStr().arg(Company::CUSTOMER);

    } else if (index == 1) {
        QString types = QString("%1, %2").arg(Company::SUPPLIER).arg(Company::SUBORDER);
        str = model->getQueryStr().arg(types);
    }

    query.prepare(str);
    query.exec();

    popMessage->hide();
    QApplication::restoreOverrideCursor();
    model->setQuery(query);
    ui->total->setText(model->getTotal().toString());
}

void DebtReport::on_type_currentIndexChanged(int index)
{
    if (index == 0) {
        setWindowTitle(QString::fromUtf8("Отчет: задолженность по заказчикам"));

    } else if (index == 1) {
        setWindowTitle(QString::fromUtf8("Отчет: задолженность поставщикам/подрядчикам"));

    }

    ui->total->clear();
    model->setQuery(QSqlQuery());
}
