#ifndef SIZEEDIT_H
#define SIZEEDIT_H

#include <QLineEdit>
#include "utility/size.h"

class SizeEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SizeEdit(QWidget *parent = 0);
    Size getSize();
    
signals:
    
public slots:
    
};

#endif // SIZEEDIT_H
