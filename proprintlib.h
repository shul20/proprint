#ifndef PROPRINTLIB_H
#define PROPRINTLIB_H

#include <QString>
#include <QStringList>
#include <QVariant>
#include <QMessageBox>
#include <QComboBox>
#include <QTableView>
#include "models/entitymodel.h"

namespace PPLib
{
    static const int maxProcess = 32;
    static const int rollLength = 50000;
    static const int rollLengthM = 50;

    const static int idRole = Qt::UserRole + 1,
                     entityRole = Qt::UserRole + 2,
                     addRole = Qt::UserRole + 3,
                     sortRole = Qt::UserRole + 4,
                     refreshRole = Qt::UserRole + 5,
                     delRole = Qt::UserRole + 6,
                     uniqueRole = Qt::UserRole + 7;


    const static int userRole = Qt::UserRole + 10;

    enum ForOrder{DELETE = 0,  PREPARE = 1,
                 TOWORK = 2, PAUSEPRINT = 3,
                 PRINT = 4, PAUSEPOSTPRINT = 5, POSTPRINT = 6, READY = 7,
                 ARCHIVE = 8,
                 COLORPROOF = 9,
                 COLORPROOFREADY = 10
                 };

    static const QStringList orderStatuses = QStringList()
            << QString::fromUtf8("Удален")
            << QString::fromUtf8("Создание")
            << QString::fromUtf8("В работу")
            << QString::fromUtf8("Пауза печать")
            << QString::fromUtf8("Печать")
            << QString::fromUtf8("Пауза пост печать")
            << QString::fromUtf8("Постпечать")
            << QString::fromUtf8("Готов")
            << QString::fromUtf8("Архив")
            << QString::fromUtf8("Цветопроба")
            << QString::fromUtf8("Цветопроба готова");


    static const QString moneyRx("[0-9]{,7}[.][0-9]{2}");
    static const QString dateTemplate("dd.MM.yyyy");
    static const QString dateTimeTemplate("dd.MM.yyyy hh:mm");
    static const QString emptyIconTemplate("<html><head/><body><p><img src=\":/img/empty-icon.png\" width = \"22\" height = \"18\" /></p></body></html>");
    static const QString importantIconTemplate("<html><head/><body><p><img src=\":/img/important-icon.png\" width = \"22\" height = \"18\" /></p></body></html>");

    void fillFormMessage();
    void accessDeniedMessage();
    int delRowMessage();
    int calcRollsAmount(int width, int size);
    void sqlError();

    bool checkBit(QVariant bitNumber, QVariant bits);
    bool checkBits(QVariant bits, QVariant bitNumber);

    //void fillProcessCombo(QComboBox *box);
    //void fillStockCombo(QComboBox *box);
}



#endif // PROPRINTLIB_H
