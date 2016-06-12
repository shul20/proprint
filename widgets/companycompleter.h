#ifndef COMPANYCOMPLETER_H
#define COMPANYCOMPLETER_H

#include <QWidget>
#include <QListView>
#include <QLineEdit>
#include "models/companymodel.h"

class CompanyCompleter : public QWidget
{
    Q_OBJECT
public:
    explicit CompanyCompleter(QWidget *parent = 0);
    void setText(const QString &text);
    QString text();
    void drawList();
    
private slots:
    void on_textChanged(const QString &text);
    void completeText(const QModelIndex &index);

private:
    QLineEdit *lineEdit;
    QListView *listView;

    CompanyModel *companyModel;

    bool eventFilter(QObject *o, QEvent *e);
};

#endif // COMPANYCOMPLETER_H
