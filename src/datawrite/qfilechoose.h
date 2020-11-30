#ifndef QFILECHOOSE_H
#define QFILECHOOSE_H

#include "../mainwindow.h"
#include "ui_mainwindow.h"

#include "../self_class.h"

class qfilechoose
{
private:
    MainWindow *ui;
public:
    qfilechoose(MainWindow *ui);
    bool filechoose();

};

#endif // QFILECHOOSE_H
