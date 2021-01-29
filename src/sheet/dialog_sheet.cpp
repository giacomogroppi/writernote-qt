#include "dialog_sheet.h"
#include "ui_dialog_sheet.h"

dialog_sheet::dialog_sheet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_sheet)
{
    ui->setupUi(this);
}

dialog_sheet::~dialog_sheet()
{
    delete ui;
}
