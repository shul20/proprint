#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QDialog>

namespace Ui {
class PopUpWindow;
}

class PopUpWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit PopUpWindow(QWidget *parent, const QString &text);
    ~PopUpWindow();
    
private:
    Ui::PopUpWindow *ui;

};

#endif // POPUPWINDOW_H
