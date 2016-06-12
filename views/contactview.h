#ifndef CONTACTVIEW_H
#define CONTACTVIEW_H

#include <QWidget>
#include "models/sortfiltermodel.h"
#include "models/contactmodel.h"

namespace Ui {
class ContactView;
}

class ContactView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ContactView(QWidget *parent = 0);
    ~ContactView();

    void setCompany(int companyId);
    bool update(int companyId);
    void exitNoUpdate();
    
private:
    Ui::ContactView *ui;
    SortFilterModel *contactF;
    ContactModel *newContacts;
    QMenu *menu;

    void appendMenu();

private slots:
    void addContact();
    void editContact();
    void editContact(QModelIndex index);
    void delContact();
    void on_table_customContextMenuRequested(const QPoint &pos);
};

#endif // CONTACTVIEW_H
