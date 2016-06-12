#ifndef PRINTERWIDGET_H
#define PRINTERWIDGET_H

#include <QWidget>
#include "models/modelmanager.h"
#include "models/sortfiltermodel.h"

namespace Ui {
class PrinterWidget;
}

class PrinterWidget : public QWidget
{
    Q_OBJECT
    
public:

    explicit PrinterWidget(QWidget *parent = 0);
    ~PrinterWidget();
    void setPrinter(int printerId);
    bool update(int printerId);
    void exitNoUpdate();

    
private slots:
    void on_modes_clicked(const QModelIndex &index);

    void on_passes_clicked(const QModelIndex &index);

    void addMode();
    void addPass();
    void addProfile();

private:
    Ui::PrinterWidget *ui;
    ModelManager *models;
    SortFilterModel *modesF, *passesF, *profilesF;

    bool passesUpdate(int modeId);
    bool profilesUpdate(int passId);

    static int counter;

};

#endif // PRINTERWIDGET_H
