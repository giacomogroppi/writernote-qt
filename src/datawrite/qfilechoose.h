#ifndef QFILECHOOSE_H
#define QFILECHOOSE_H

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "../self_class.h"

#define TYPEFILEWRITER 0
#define TYPEFILEPDF 1


class qfilechoose
{
private:
    MainWindow *ui;
public:
    qfilechoose(MainWindow *ui);
    bool filechoose(QString *, short int type_ = TYPEFILEWRITER);

};

#endif // QFILECHOOSE_H
