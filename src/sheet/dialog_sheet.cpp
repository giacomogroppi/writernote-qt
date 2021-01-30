#include "dialog_sheet.h"
#include "ui_dialog_sheet.h"

#include "load_last_style.h"

dialog_sheet::dialog_sheet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_sheet)
{
    ui->setupUi(this);

    style_struct *style_last_save = load_last_style();

}

dialog_sheet::~dialog_sheet()
{
    delete ui;
}
