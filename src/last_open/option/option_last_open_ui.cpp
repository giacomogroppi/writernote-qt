#include "option_last_open_ui.h"
#include "ui_option_last_open_ui.h"
#include <QSettings>
#include "../../utils/setting_define.h"

option_last_open_ui::option_last_open_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::option_last_open_ui)
{
    ui->setupUi(this);

    ui->pushButton_always_enable->setCheckable(true);
    ui->pushButton_enable->setCheckable(true);
    ui->pushButton_disable->setCheckable(true);

    loadData();
}

option_last_open_ui::~option_last_open_ui()
{
    delete ui;
}

void option_last_open_ui::saveData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    setting.setValue(KEY_LAST_FILE_METHOD, data.val);
    setting.setValue(KEY_LAST_FILE_NUMBER, data.pos);

    setting.endGroup();
}

void option_last_open_ui::loadData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    bool ok_t;

    data.val = static_cast<option>(
                setting.value(KEY_LAST_FILE_METHOD, option::enable).toInt(&ok_t)
                );

    if(!ok_t){
        data.val = option::enable;
    }

    data.pos = setting.value(KEY_LAST_FILE_NUMBER, option_last_open_ui_def_num).toChar().toLatin1();


    setting.endGroup();

    updateShow();
}

void option_last_open_ui::updateShow()
{
    ui->spinBox->setValue(data.pos);

    ui->spinBox->setEnabled(data.val == option::enable);

    ui->pushButton_always_enable->setChecked(data.val == option::open_last);
    ui->pushButton_disable->setChecked(data.val == option::disable);
    ui->pushButton_enable->setChecked(data.val == option::enable);
}

void option_last_open_ui::on_pushButton_enable_clicked()
{
    data.val = option::enable;
    updateShow();
}

void option_last_open_ui::on_pushButton_always_enable_clicked()
{
    data.val = option::open_last;
    updateShow();
}

void option_last_open_ui::on_pushButton_disable_clicked()
{
    data.val = option::disable;
    updateShow();
}

void option_last_open_ui::on_ok_button_clicked()
{
    data.pos = ui->spinBox->value();

    emit changeSomething(data.val, data.pos);
}

void option_last_open_ui::on_cancel_button_clicked()
{
    this->close();
}


void option_last_open_ui::on_spinBox_valueChanged(int arg1)
{
    if(!arg1){
        on_pushButton_disable_clicked();
    }
}
