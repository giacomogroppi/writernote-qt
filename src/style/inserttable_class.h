#ifndef INSERTTABLE_CLASS_H
#define INSERTTABLE_CLASS_H
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>

namespace Ui {
class inserttable_class;
}

class inserttable_class : public QDialog
{
    Q_OBJECT

public:
    explicit inserttable_class(QWidget *parent = nullptr, Ui::MainWindow *padre = nullptr);
    ~inserttable_class();

    Ui::MainWindow *padre;
private slots:
    void on_pushButton_clicked();

private:
    Ui::inserttable_class *ui;
};

#endif // INSERTTABLE_CLASS_H
