#include "areyousure.h"
#include <QString>
#include <QMessageBox>


bool areyousure(MainWindow *parent, QString title, QString body){
    QMessageBox::StandardButton resBtn = QMessageBox::question( parent, title,
                                                                body,
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);

    if(resBtn == QMessageBox::Yes)
        return true;

    return false;
}
