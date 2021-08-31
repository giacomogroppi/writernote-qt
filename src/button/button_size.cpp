#include "button_size.h"
#include "ui_button_size.h"

button_size::button_size(QWidget *parent, int size) :
    QDialog(parent),
    ui(new Ui::button_size)
{
    ui->setupUi(this);
    ui->horizontalSlider->setMinimum(5);
    ui->horizontalSlider->setMaximum(100);

    ui->horizontalSlider->setValue(size);
}

button_size::~button_size()
{
    delete ui;
}

void button_size::on_pushButton_clicked()
{
    this->close();
}

void button_size::on_horizontalSlider_valueChanged(int value)
{
    emit update_size(value);
}

