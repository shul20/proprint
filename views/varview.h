#ifndef VARVIEW_H
#define VARVIEW_H

#include <QWidget>

namespace Ui {
class VarView;
}

class VarView : public QWidget
{
    Q_OBJECT
    
public:
    explicit VarView(QWidget *parent = 0);
    ~VarView();
    
private:
    Ui::VarView *ui;
};

#endif // VARVIEW_H
