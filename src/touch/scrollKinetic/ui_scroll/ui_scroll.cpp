#include "ui_scroll.h"

#include <QDebug>
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

void ui_scroll::on_spinBox_valueChanged(int arg1)
{
    bool __app = true;
    if(arg1 <= 0){
        ui->spinBox->setValue(1);
        __app = false;
    }

    ui->checkBox->setChecked(__app);
    if(!__app){
        ui->spinBox->setEnabled(false);
    }
}

void ui_scroll::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked()){
        ui->spinBox->setEnabled(true);

        if(!ui->spinBox->value()){
            ui->spinBox->setValue(1);
        }
    }else{
        ui->spinBox->setEnabled(false);
    }
}
