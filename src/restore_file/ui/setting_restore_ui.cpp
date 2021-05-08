#include "setting_restore_ui.h"
#include "ui_setting_restore_ui.h"

setting_restore_ui::setting_restore_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting_restore_ui)
{
    ui->setupUi(this);
}

setting_restore_ui::~setting_restore_ui()
{
    delete ui;
}
