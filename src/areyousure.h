#ifndef ARE_YOU_SURE_CPP
#define ARE_YOU_SURE_CPP

#include <QString>
#include <QDialog>
#include <QMessageBox>
#include "mainwindow.h"

bool areyousure(MainWindow *parent, QString title, QString body){
    QMessageBox::StandardButton resBtn = QMessageBox::question( parent, title,
                                                                body,
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if(resBtn == QMessageBox::Yes)
        return true;

    return false;
}


#endif
