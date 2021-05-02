#include "showmessageupdate.h"
#include "ui_showmessageupdate.h"

ShowMessageUpdate::ShowMessageUpdate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowMessageUpdate)
{
    ui->setupUi(this);
}

ShowMessageUpdate::~ShowMessageUpdate()
{
    delete ui;
}
