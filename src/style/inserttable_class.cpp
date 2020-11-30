#include "ui_inserttable_class.h"
#include "inserttable_class.h"
#include "../mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QTextCursor>

inserttable_class::inserttable_class(QWidget *parent, Ui::MainWindow *padre) :
    QDialog(parent),
    ui(new Ui::inserttable_class)
{
    ui->setupUi(this);

    this->padre = padre;

    this->ui->padding_spinbox->setValue(10);
}

inserttable_class::~inserttable_class()
{
    delete ui;
}

/* pulsante insert cliccato */
void inserttable_class::on_pushButton_clicked()
{
    QTextCursor cursor;
    cursor = this->padre->textEdit->textCursor();

    int numerorows = this->ui->rows_spinbox->value();
    int numerocols = this->ui->columns_spinbox->value();

    if(!numerorows || !numerocols)
    {
        this->close();
        return;
    }
    int padding = this->ui->padding_spinbox->value();
    int space = this->ui->spacing_spinbox->value();

    QTextTableFormat fmt;
    fmt.setCellPadding(padding);
    fmt.setCellSpacing(space);

    cursor.insertTable(numerorows, numerocols, fmt);
    this->close();
}

