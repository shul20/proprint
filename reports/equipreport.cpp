#include "equipreport.h"
#include "ui_equipreport.h"
#include "models/equip/equipmentmodel.h"
#include "utility/process.h"
#include "widgets/entitytable.h"
#include <QDebug>
#include "utility/size.h"
#include "widgets/popupwindow.h"
#include <QTimer>

EquipReport::EquipReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EquipReport)
{
    ui->setupUi(this);
    ui->equip->setup(true, false, false, false);
    ui->equip->setEquipRole(-1, Process::PRINT);

    setWindowTitle(QString::fromUtf8("Отчет: использованные материалы"));

    EntityTable::setup(ui->table);
    ui->table->setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QTableView::item {margin-right: 20; margin-left: 20;}");

    QDate current = QDate::currentDate();
    ui->dateTill->setDate(current);
    ui->dateFrom->setDate(current.addMonths(-1));

    model = new EquipReportM(this);
    ui->table->setModel(model);
    popMessage = new PopUpWindow(this, QString::fromUtf8("создание отчета"));
}

EquipReport::~EquipReport()
{
    delete ui;
}

void EquipReport::on_startButton_clicked()
{    
    popMessage->show();
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTimer::singleShot(100, this, SLOT(execute()));
}

void EquipReport::execute()
{
    Equipment *equip = ui->equip->getEquip();

    QSqlQuery &query = model->getQuery();

    query.bindValue(":id", equip->getId());
    query.bindValue(":start", ui->dateFrom->date());
    query.bindValue(":end", ui->dateTill->date());

    query.exec();

    popMessage->hide();
    QApplication::restoreOverrideCursor();

    model->setQuery(query);

    ui->sizeTotal->setText(model->getTotalSize().toString());
}
