#include <QMenuBar>
#include <QToolBar>
#include "entity/client.h"
#include "mainwindow.h"

#include "views/clientview.h"
#include "views/companyview.h"
#include "views/productview.h"
#include "views/stock/stockview.h"
#include "views/stock/priceview.h"
#include "views/equipview.h"
#include "views/varview.h"
#include "models/varmodel.h"
#include "dialogs/currencyratedialog.h"
#include "views/workflow.h"
#include "views/archiveview.h"
#include "views/finance/payview.h"
#include "views/finance/paymentview.h"
#include "views/stock/woffview.h"
#include "views/msgview.h"
#include "reports/equipreport.h"
#include "reports/debtreport.h"
#include "reports/companyreport.h"
#include "models/modelmanager.h"

QMenu *operate, *admin, *report,*help;

void MainWindow::setupMenu(QMenuBar *menuBar, QToolBar *toolBar){

    operate = new QMenu(menuBar);
    operate->setTitle(QString::fromUtf8("Процесс"));

    menuBar->addAction(operate->menuAction());

    QAction *actionWorkFlow = new QAction(QString::fromUtf8("Рабочий процесс"), operate);
    actionWorkFlow->setIcon(QIcon(":img/order-icon.png"));
    operate->addAction(actionWorkFlow);
    toolBar->addAction(actionWorkFlow);
    connect(actionWorkFlow, SIGNAL(triggered()), this, SLOT(openWorkFlow()));

    QAction *actionOrderArch = new QAction(QString::fromUtf8("Заказы архив"), operate);
    actionOrderArch->setIcon(QIcon(":img/order-archive.icon.png"));
    connect(actionOrderArch, SIGNAL(triggered()), this, SLOT(openOrdersArch()));

    QAction *actionMessage = new QAction(QString::fromUtf8("Список задач"), operate);
    connect(actionMessage, SIGNAL(triggered()), this, SLOT(openMessage()));


    operate->addAction(actionOrderArch);
    operate->addAction(actionMessage);

    if (Client::currentRole == Client::MANAGER ||
        Client::currentRole == Client::ADMIN) {

        QAction *actionStock = new QAction(QString::fromUtf8("Склад"), operate);
        actionStock->setIcon(QIcon(":img/stock-icon.png"));
        operate->insertAction(actionOrderArch, actionStock);
        toolBar->addAction(actionStock);
        connect(actionStock, SIGNAL(triggered()), this, SLOT(openStock()));

        QAction *actionWoff = new QAction(QString::fromUtf8("Списание"), operate);
        operate->insertAction(actionOrderArch, actionWoff);
        connect(actionWoff, SIGNAL(triggered()), this, SLOT(openWoff()));

        QAction *actionPayment = new QAction(QString::fromUtf8("Платежи"), operate);
        actionPayment->setIcon(QIcon(":img/payment-icon.png"));
        operate->insertAction(actionOrderArch, actionPayment);
        toolBar->addAction(actionPayment);
        connect(actionPayment, SIGNAL(triggered()), this, SLOT(openPayment()));

        QAction *actionPrice = new QAction(QString::fromUtf8("Прайс"), operate);
        operate->addAction(actionPrice);
        connect(actionPrice, SIGNAL(triggered()), this, SLOT(openPrice()));
    }

    toolBar->addAction(actionOrderArch);

    if (Client::currentRole == Client::ADMIN) {

        admin = new QMenu(menuBar);
        admin->setTitle(QString::fromUtf8("Управление"));

        menuBar->addAction(admin->menuAction());

        QAction *actionRate = new QAction(QString::fromUtf8("Курс"), admin);
        admin->addAction(actionRate);
        connect(actionRate, SIGNAL(triggered()), this, SLOT(setRate()));

        QAction *actionClients = new QAction(QString::fromUtf8("Пользователи"), admin);
        admin->addAction(actionClients);
        connect(actionClients, SIGNAL(triggered()), this, SLOT(openClients()));

        QAction *actionProduct = new QAction(QString::fromUtf8("Продукция"), admin);
        admin->addAction(actionProduct);
        connect(actionProduct, SIGNAL(triggered()), this, SLOT(openProducts()));

        QAction *actionEquip = new QAction(QString::fromUtf8("Оборудование"), admin);
        admin->addAction(actionEquip);
        connect(actionEquip, SIGNAL(triggered()), this, SLOT(openEquip()));

        QAction *actionCompany = new QAction(QString::fromUtf8("Компании"), admin);
        admin->addAction(actionCompany);
        connect(actionCompany, SIGNAL(triggered()), this, SLOT(openCompany()));

        QAction *actionPay = new QAction(QString::fromUtf8("Конструктор платежей"), operate);
        admin->insertAction(actionOrderArch, actionPay);
        connect(actionPay, SIGNAL(triggered()), this, SLOT(openPay()));

        admin->addSeparator();
        QAction *actionVar = new QAction(QString::fromUtf8("Переменные"), admin);
        admin->addAction(actionVar);
        connect(actionVar, SIGNAL(triggered()), this, SLOT(openVar()));

        report = new QMenu(menuBar);
        report->setTitle(QString::fromUtf8("Отчеты"));

        QMenu *finMenu = new QMenu(report);
        finMenu->setTitle(QString::fromUtf8("Финансы"));

        report->addMenu(finMenu);

        QAction *debtReportAct = new QAction(QString::fromUtf8("Задолженность"), admin);
        finMenu->addAction(debtReportAct);
        connect(debtReportAct, SIGNAL(triggered()), this, SLOT(debtReport()));

        QAction *companyReportAct = new QAction(QString::fromUtf8("Компания"), admin);
        finMenu->addAction(companyReportAct);
        connect(companyReportAct, SIGNAL(triggered()), this, SLOT(companyReport()));


        QAction *equipReportAction = new QAction(QString::fromUtf8("Оборудование"), admin);
        report->addAction(equipReportAction);
        connect(equipReportAction, SIGNAL(triggered()), this, SLOT(equipReport()));

        menuBar->addAction(report->menuAction());
    }

    help = new QMenu(menuBar);
    help->setTitle(QString::fromUtf8("Помощь"));
    menuBar->addAction(help->menuAction());

    QAction *actionAbout = new QAction(QString::fromUtf8("О программе"), help);
    help->addAction(actionAbout);
    connect(actionAbout, SIGNAL(triggered()), this, SLOT(openAbout()));
}

void MainWindow::clearMenu(QMenuBar *menuBar, QToolBar *toolBar){
    if (operate != NULL) {
        menuBar->removeAction(operate->menuAction());
        delete operate;
        operate = NULL;
    }

    if (admin != NULL) {
        menuBar->removeAction(admin->menuAction());
        delete admin;
        admin = NULL;
    }

    if (report != NULL) {
        menuBar->removeAction(report->menuAction());
        delete report;
        report = NULL;
    }

    menuBar->removeAction(help->menuAction());
    delete help;

    toolBar->clear();
}

void MainWindow::openWorkFlow()
{
    if (area->subWindowList().contains(workFlowWindow)) {
         workFlowWindow->setFocus();
         return;
    }

     WorkFlow * workFlowView = new WorkFlow(this);
     workFlowWindow = area->addSubWindow(workFlowView);
     workFlowWindow->show();
     workFlowWindow->setGeometry(0, 0, area->width(), area->height());
}

void MainWindow::openOrdersArch()
{
    if (area->subWindowList().contains(archiveWindow)) {
        archiveWindow->setFocus();
        return;
   }

    ArchiveView * archiveView = new ArchiveView(this);
    archiveWindow = area->addSubWindow(archiveView);
    archiveWindow->show();
    archiveWindow->setGeometry(0, 0, area->width(), area->height());
}

void MainWindow::openStock()
{
   if (area->subWindowList().contains(stockWindow)) {
            stockWindow->setFocus();
            return;
        }

    StockView * stockView = new StockView(this);
    stockWindow = area->addSubWindow(stockView);
    stockWindow->show();
    stockWindow->setGeometry(0, 0, area->width(), area->height());
}

void MainWindow::openWoff()
{
    if (area->subWindowList().contains(woffWindow)) {
        woffWindow->setFocus();
        return;
   }

    WoffView * woffView = new WoffView(this);
    woffWindow = area->addSubWindow(woffView);
    woffWindow->show();
    woffWindow->setGeometry(0, 0, area->width(), area->height());

}


void MainWindow::openPayment()
{
    if (area->subWindowList().contains(paymentWindow)) {
        paymentWindow->setFocus();
        return;
   }

    PaymentView * payView = new PaymentView(this);
    paymentWindow = area->addSubWindow(payView);
    paymentWindow->show();
    paymentWindow->setGeometry(0, 0, area->width(), area->height());
}

void MainWindow::openPay()
{
    int width = 600, height = 400;
    if (area->subWindowList().contains(payWindow)) {
        payWindow->setFocus();
        return;
   }

    PayView * payView = new PayView(this);
    payWindow = area->addSubWindow(payView);
    payWindow->show();
    payWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
                              width, height);
}

void MainWindow::setRate()
{
    CurrencyRateDialog dialog;
    dialog.exec();
}

void MainWindow::openClients()
{
    int width = 400, height = 400;

    if (area->subWindowList().contains(clientWindow)) {
            clientWindow->setFocus();
            return;
        }

    ClientView * clientView = new ClientView(this);
    clientWindow = area->addSubWindow(clientView);
    clientWindow->show();
    clientWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
                              width, height);
}

void MainWindow::openProducts()
{
    int width = 400, height = 400;
    if (area->subWindowList().contains(productWindow)) {
            productWindow->setFocus();
            return;
        }

    ProductView * productView = new ProductView(this);
    productWindow = area->addSubWindow(productView);
    productWindow->show();

    productWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
                              width, height);

}

void MainWindow::openEquip()
{
    int width = 400, height = 400;

    if (area->subWindowList().contains(equipWindow)) {
         equipWindow->setFocus();
         return;
    }

     EquipView * equipView = new EquipView(this);
     equipWindow = area->addSubWindow(equipView);
     equipWindow->show();

     equipWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
                               width, height);
}

void MainWindow::openCompany()
{
   int width = 600, height = 400;
   if (area->subWindowList().contains(companyWindow)) {
            companyWindow->setFocus();
            return;
        }

    CompanyView * companyView = new CompanyView(this);
    companyWindow = area->addSubWindow(companyView);
    companyWindow->show();

    companyWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
                              width, height);
}

void MainWindow::openPrice()
{
    //int width = 600, height = 400;
    if (area->subWindowList().contains(priceWindow)) {
         priceWindow->setFocus();
         return;
    }

     PriceView * priceView = new PriceView(this);
     priceWindow = area->addSubWindow(priceView);
     priceWindow->show();
     priceWindow->setGeometry(0, 0, area->width(), area->height());
     //priceWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
       //                        width, height);
}

void MainWindow::openVar()
{
    int width = 600, height = 400;
    if (area->subWindowList().contains(varWindow)) {
         varWindow->setFocus();
         return;
    }

     VarView * varView = new VarView(this);
     varWindow = area->addSubWindow(varView);
     varWindow->show();
     varWindow->setGeometry((area->width() - width) / 2, (area->height() - height) / 2,
                               width, height);
}

void MainWindow::openMessage()
{
    if (area->subWindowList().contains(msgWindow)) {
        msgWindow->setFocus();
        return;
   }

    MsgView *msgView = new MsgView(this);
    msgWindow = area->addSubWindow(msgView);
    msgWindow->show();
    msgWindow->setGeometry(0, 0, area->width(), area->height());

}

void MainWindow::openAbout()
{
    const QString version = QString::fromUtf8("программа на стадии: разработка\n версия: %1.%2 ")
            .arg(MainWindow::version/100).arg(MainWindow::version%100, 2, 10, QChar('0'));
    QMessageBox::information(this, QString::fromUtf8("Версия программы"), version);
}

void MainWindow::equipReport(){
    if (area->subWindowList().contains(equipReportWindow)) {
        equipReportWindow->setFocus();
        return;
   }

    EquipReport *equipReport = new EquipReport(this);
    equipReportWindow = area->addSubWindow(equipReport);
    equipReportWindow->show();
    equipReportWindow->setGeometry(0, 0, area->width(), area->height());
}

void MainWindow::debtReport()
{
    int width = 600, height = 400;
    if (area->subWindowList().contains(debtReportWindow)) {
        debtReportWindow->setFocus();
        return;
   }

    DebtReport *debtReport = new DebtReport(this);
    debtReportWindow = area->addSubWindow(debtReport);
    debtReportWindow->show();
    debtReportWindow->setGeometry((area->width() - width) / 2,
                                  (area->height() - height) / 2,
                                  width, height);
}


void MainWindow::companyReport()
{
    int width = 800, height = 600;
    if (area->subWindowList().contains(companyReportWindow)) {
        companyReportWindow->setFocus();
        return;
   }

    CompanyReport *companyReport = new CompanyReport(this);
    companyReportWindow = area->addSubWindow(companyReport);
    companyReportWindow->show();
    companyReportWindow->setGeometry((area->width() - width) / 2,
                                  (area->height() - height) / 2,
                                  width, height);
}
