#include "option_last_open_ui.h"
#include "ui_option_last_open_ui.h"
#include <QSettings>
#include "utils/setting_define.h"
#include "utils/WCommonScript.h"
#include "core/core.h"

option_last_open_ui::option_last_open_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::option_last_open_ui)
{
    ui->setupUi(this);

    ui->pushButton_always_enable->setCheckable(true);
    ui->pushButton_enable->setCheckable(true);
    ui->pushButton_disable->setCheckable(true);

    ui->spinBox->setMaximum(126);
    ui->spinBox->setMinimum(1);

#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    ui->showOnlyName->setHidden(true);
    ui->checkBox->setHidden(true);
#endif

    loadData();

    core::set_max_size_as_screen(this);
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
#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    setting.setValue(KEY_LAST_FILE_SHOW_ONLY_NAME, data.showOnlyName);
#endif
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

    data.pos = setting.value(KEY_LAST_FILE_NUMBER, option_last_open_ui_def_num).toInt(&ok_t);
    if(!ok_t || data.pos <= 0)
        data.pos = option_last_open_ui_def_num;

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    data.showOnlyName = setting.value(KEY_LAST_FILE_SHOW_ONLY_NAME, false).toBool();
    data.showFileIfExist = setting.value(KEY_LAST_FILE_SHOW_FILE_IF_EXIST, false).toBool();
#endif
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

    ui->showOnlyName->setChecked(data.showOnlyName);
}

void option_last_open_ui::on_pushButton_enable_clicked()
{
    data.val = option::enable;

    if(!ui->spinBox->value()){
        on_spinBox_valueChanged(1);
    }

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

    saveData();

    this->close();
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

    data.pos = arg1;

}

void option_last_open_ui::on_showOnlyName_stateChanged(int arg1)
{
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    Q_UNUSED(arg1);
#else
    this->data.showOnlyName = arg1;
#endif
}


void option_last_open_ui::on_checkBox_stateChanged(int arg1)
{
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    Q_UNUSED(arg1);
#else
    this->data.showFileIfExist = arg1;
#endif
}

