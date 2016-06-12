#include "proprintlib.h"
#include "utility/size.h"
#include <QSqlError>
#include "mainwindow.h"
//#include <QHeaderView>


void PPLib::fillFormMessage(){
    QMessageBox::critical(NULL, QString::fromUtf8("Ошибка"),
       QString::fromUtf8("Заполните форму"), QMessageBox::Ok);
}

void PPLib::accessDeniedMessage(){
    QMessageBox::critical(NULL, QString::fromUtf8("Блокировка"),
       QString::fromUtf8("Доступ закрыт"), QMessageBox::Ok);
}

int PPLib::calcRollsAmount(int width, int size)
{
    return qRound((qreal)size / (qreal)Size(width, rollLength).value);

}


void PPLib::sqlError()
{
    QSqlError error = MainWindow::db->lastError();
    qDebug() << error.databaseText();
    qDebug() << error.driverText();
}


int PPLib::delRowMessage()
{
    return QMessageBox::question(NULL, QString::fromUtf8("Удаление"),
                                 QString::fromUtf8("Удалить выбранные строки?"),
                                 QMessageBox::Cancel | QMessageBox::Ok);

}

bool PPLib::checkBit(QVariant bitNumber, QVariant bits) {
    return (bool)((bits.toInt() >> bitNumber.toInt()) & 1);
}

bool PPLib::checkBits(QVariant bits, QVariant bitNumber) {
    return (bool)((bits.toInt() >> bitNumber.toInt()) & 1);
}
