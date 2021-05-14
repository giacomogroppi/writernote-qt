#include "ui_restore.h"
#include "ui_ui_restore.h"

ui_restore::ui_restore(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_restore)
{
    ui->setupUi(this);
}

ui_restore::~ui_restore()
{
    delete ui;
}
