#include "cloud_ui.h"
#include "ui_cloud_ui.h"

cloud_ui::cloud_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cloud_ui)
{
    ui->setupUi(this);
}

cloud_ui::~cloud_ui()
{
    delete ui;
}

void cloud_ui::on_pushButton_clicked()
{
    this->hide();
}
