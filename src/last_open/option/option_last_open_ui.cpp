#include "option_last_open_ui.h"
#include "ui_option_last_open_ui.h"
#include <QSettings>
#include "../../utils/setting_define.h"

option_last_open_ui::option_last_open_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::option_last_open_ui)
{
    ui->setupUi(this);



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
}

void option_last_open_ui::updateShow()
{

}

void option_last_open_ui::on_pushButton_enable_clicked()
{

}

void option_last_open_ui::on_pushButton_always_enable_clicked()
{

}

void option_last_open_ui::on_pushButton_disable_clicked()
{

}

void option_last_open_ui::on_ok_button_clicked()
{

}

void option_last_open_ui::on_cancel_button_clicked()
{
    this->close();
}

