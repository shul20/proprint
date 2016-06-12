#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "setupmenu.cpp"
#include "models/modelmanager.h"


QMdiArea *MainWindow::area = NULL;
QSqlDatabase *MainWindow::db = NULL;
MainWindow *MainWindow::mainWindow = NULL;
const int MainWindow::version = 8;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    status = new Status(this);
    ui->statusBar->addWidget(status);

    showMaximized();

    db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    on_actionConnect_triggered();

    area = ui->mdiArea;
    mainWindow = this;

    QDesktopWidget widget;
    mainScreenSize = widget.availableGeometry(widget.primaryScreen());
}

MainWindow::~MainWindow()
{
    delete ui;
}

QMdiArea *MainWindow::getMdiArea(){
    return ui->mdiArea;
}

void MainWindow::on_actionConnect_triggered()
{
    if (db->isOpen()) {
        return;
    }

#ifndef QT_DEBUG
    LogInDialog dialog(this, db);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }

#endif //QT_DEBUG


#ifdef QT_DEBUG
    QString user = QString::fromLocal8Bit("shul20");
    db->setHostName("37.53.75.87");
    db->setDatabaseName("pro_print");
    db->setUserName(user);
    db->setPassword(" ");
    //db->setConnectOptions("requiressl=1");
    if (!db->open()) {
        //db->setConnectOptions();
        return;
    }
    Client *client = new Client();
    client->retrive("client_name", user);
    Client::currentClient = client;
    Client::currentRole = client->fields["client_role"].toInt();
#endif

    checkVersion();

    status->start();

    setupMenu(ui->menuBar, ui->mainToolBar);


#ifndef QT_DEBUG
    /*if (Client::clientRole < UserModel::PRINT) {
        on_setCurrency_triggered();
    }*/
#endif

}

bool MainWindow::checkVersion()
{
   VarModel *var = ModelManager::getInstance()->getVarModel();
   int actualVersion = var->getVar(Var::VERSION).toInt();
   if (actualVersion != version) {
       QString ver = QString("%1.%2").arg(version/100).arg(version%100, 2, 10, QChar('0'));
       QString actual = QString("%1.%2").arg(actualVersion/100).arg(actualVersion%100, 2, 10, QChar('0'));

        QMessageBox::information(this, QString::fromUtf8("Обновите версию программы"),
                         QString::fromUtf8(
                         "Текущая версия: %1\n"
                         "Актуальная версия: %2").arg(ver).arg(actual));
        return false;
   }
   return true;
}

void MainWindow::on_actionDisconnect_triggered()
{
    if (!db->isOpen()) {
        return;
    }
    db->close();
    status->clear();
    clearMenu(ui->menuBar, ui->mainToolBar);
}

void MainWindow::on_actionQuit_triggered()
{
    db->close();
    close();
}
