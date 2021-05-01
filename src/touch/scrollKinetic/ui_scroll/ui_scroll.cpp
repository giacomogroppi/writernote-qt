#include "ui_scroll.h"
#include "ui_ui_scroll.h"

ui_scroll::ui_scroll(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_scroll)
{
    ui->setupUi(this);

}

ui_scroll::~ui_scroll()
{
    delete ui;
}

void ui_scroll::reset(uint enable, int speed)
{
    ui->checkBox->setChecked(enable);
    ui->spinBox->setValue(speed);
}

void ui_scroll::on_pushButton_cancel_clicked()
{
    this->close();
}

void ui_scroll::on_pushButton_ok_clicked()
{
    emit updateData((uchar)ui->checkBox->isChecked(),
                    ui->spinBox->value());
    this->close();
}
