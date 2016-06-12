#ifndef BASEGROUPBOX_H
#define BASEGROUPBOX_H

#include <QGroupBox>
#include <QMouseEvent>

class BaseGroupBox : public QGroupBox
{
    Q_OBJECT
private:
    bool isBlock;

public:
    explicit BaseGroupBox(QWidget *parent = 0);
    void block(bool isBlock);

protected:

    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *e);

signals:
    
public slots:
    
};

#endif // BASEGROUPBOX_H
