#ifndef COMPANYDIALOG_H
#define COMPANYDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "models/entitymodel.h"
#include "entity/company.h"
#include "models/contactmodel.h"
#include "views/contactview.h"
#include "models/companymodel.h"
#include <QCompleter>

class CompanyModel;

namespace Ui {
class CompanyDialog;
}

class CompanyDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CompanyDialog( QWidget *parent);
    ~CompanyDialog();
    void set(QAbstractItemModel *model, int mode, const QModelIndex &index);
    void disableCompanyType(bool check);
    void clear();
private slots:
    void on_saveCompanyButton_clicked();


    void on_exitCompanyButton_clicked();

private:
    Ui::CompanyDialog *ui;
    ContactView *contacts;
    QAbstractItemModel *model;
    CompanyModel *completerModel;
    QCompleter *completer;
    QModelIndex index;
    Entity *view;
    int mode;

    bool form2Entity(Entity *company);
    void entity2Form(Entity *company);

    bool eventFilter(QObject *o, QEvent *e);
};

#endif // COMPANYDIALOG_H
