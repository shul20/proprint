#include "companycompleter.h"
#include <QVBoxLayout>
#include <QEvent>

CompanyCompleter::CompanyCompleter(QWidget *parent) :
    QWidget(parent)
{
    lineEdit = new QLineEdit(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(lineEdit);
    layout->setMargin(0);
    //layout->addStretch(100);
    setLayout(layout);

    companyModel = new CompanyModel(this);

    listView = new QListView(this);
    listView->setModel(companyModel);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    listView->setParent(lineEdit, Qt::ToolTip);
    listView->setFocusPolicy(Qt::NoFocus);
    listView->setFocusProxy(lineEdit);

    connect(lineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(on_textChanged(const QString &)));

    connect(listView, SIGNAL(activated(const QModelIndex &)),
        this, SLOT(completeText(const QModelIndex &)));
    connect(listView, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(completeText(const QModelIndex &)));
    connect(listView, SIGNAL(activated(const QModelIndex &)),
        this, SLOT(completeText(const QModelIndex &)));

    installEventFilter(this);

}

void CompanyCompleter::setText(const QString &text)
{
    lineEdit->setText(text);
}

QString CompanyCompleter::text()
{
    return lineEdit->text();
}

void CompanyCompleter::on_textChanged(const QString &text)
{
    QSqlQuery &query = companyModel->getRefreshQuery();
    query.prepare(Company::fuzzyQuery);
    query.bindValue(":name", text);
    companyModel->refresh();
    if (companyModel->rowCount() == 0) {
        listView->hide();
        return;
    }

    listView->show();
    drawList();
}

void CompanyCompleter::completeText(const QModelIndex &index)
{
    lineEdit->setText(index.data().toString());
    listView->hide();
}


bool CompanyCompleter::eventFilter(QObject *o, QEvent *e)
{
    if (!lineEdit->hasFocus() && !listView->hasFocus()) {
        listView->hide();
        return true;
    }

    return false;
}

void CompanyCompleter::drawList()
{
    if (listView->isHidden()) {
        return;
    }

    int lineEidtWidth = lineEdit->width();
    listView->setMinimumWidth(lineEidtWidth);
    listView->setMaximumWidth(lineEidtWidth);
    listView->setMaximumHeight(60);

    QPoint p(0, lineEdit->height());
    int x = lineEdit->mapToGlobal(p).x();
    int y = lineEdit->mapToGlobal(p).y();
    listView->move(x, y);
}
