#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QDesktopWidget>


#include "dialogs/logindialog.h"
#include "widgets/status.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    Status *status;

    QMdiSubWindow *clientWindow;
    QMdiSubWindow *productWindow;
    QMdiSubWindow *companyWindow;
    QMdiSubWindow *stockWindow;
    QMdiSubWindow *equipWindow;
    QMdiSubWindow *priceWindow;
    QMdiSubWindow *varWindow;
    QMdiSubWindow *workFlowWindow;
    QMdiSubWindow *archiveWindow;
    QMdiSubWindow *payWindow;
    QMdiSubWindow *paymentWindow;
    QMdiSubWindow *woffWindow;
    QMdiSubWindow *msgWindow;
    QMdiSubWindow *equipReportWindow;
    QMdiSubWindow *debtReportWindow;
    QMdiSubWindow *companyReportWindow;

    QRect mainScreenSize;


public:
    static QMdiArea *area;
    static QSqlDatabase *db;
    static MainWindow *mainWindow;
    static const int version;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

     QMdiArea *getMdiArea();

public slots:
    void openMessage();

private slots:
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionQuit_triggered();
    void openWorkFlow();
    void openOrdersArch();
    void openStock();
    void openWoff();
    void openPayment();
    void setRate();
    void openClients();
    void openProducts();
    void openEquip();
    void openCompany();
    void openPrice();
    void openVar();
    void openAbout();
    void equipReport();
    void debtReport();
    void companyReport();
    void openPay();


private:
    void setupMenu(QMenuBar *menuBar, QToolBar *toolBar);
    void clearMenu(QMenuBar *menuBar, QToolBar *toolBar);
    bool checkVersion();
};

#endif // MAINWINDOW_H
