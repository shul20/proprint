#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QWidget>
#include <QModelIndex>

namespace Ui {
class ClientView;
}

class ClientView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ClientView(QWidget *parent = 0);
    ~ClientView();

private:
    Ui::ClientView *ui;

private slots:
    void addClient();
    void editClient();
    void editClient(QModelIndex index);

};

#endif // CLIENTVIEW_H
