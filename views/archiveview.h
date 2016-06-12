#ifndef ARCHIVEVIEW_H
#define ARCHIVEVIEW_H

#include <QWidget>

namespace Ui {
class ArchiveView;
}

class ArchiveView : public QWidget
{
    Q_OBJECT
    
public:
    explicit ArchiveView(QWidget *parent = 0);
    ~ArchiveView();
    
private:
    Ui::ArchiveView *ui;
};

#endif // ARCHIVEVIEW_H
