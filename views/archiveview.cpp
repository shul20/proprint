#include "archiveview.h"
#include "ui_archiveview.h"

ArchiveView::ArchiveView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ArchiveView)
{
    ui->setupUi(this);

    ui->splitter->setStretchFactor(0, 2);
    ui->splitter->setStretchFactor(1, 1);

    setWindowTitle(QString::fromUtf8("Архив заказов"));
    ui->orders->setup(ui->info);
}

ArchiveView::~ArchiveView()
{
    delete ui;
}
