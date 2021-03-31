#include "copy_widgets.h"
#include "ui_copy_widgets.h"

copy_widgets::copy_widgets(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::copy_widgets)
{
    ui->setupUi(this);
}

copy_widgets::~copy_widgets()
{
    delete ui;
}
