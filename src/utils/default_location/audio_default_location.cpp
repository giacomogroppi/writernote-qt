#include "audio_default_location.h"
#include "ui_audio_default_location.h"

#include <QSettings>
#include "../setting_define.h"

#define SET_CHECK(x) ui->button_into->setChecked((x)); \
ui->button_ext->setChecked(!(x));


audio_default_location::audio_default_location(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::audio_default_location)
{
    ui->setupUi(this);

    auto res = audio_default::load_default();


    SET_CHECK(res == audio_default::external || res == audio_default::not_define);

}

audio_default_location::~audio_default_location()
{
    delete ui;
}

void audio_default_location::on_button_ok_clicked()
{
    this->save_data();
    this->close();
}

void audio_default_location::save_data(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_POSITION);

    int temp;
    if(ui->button_into->isChecked()){
        temp = audio_default::internal;
    }
    else
        temp = audio_default::external;

    setting.setValue(KEY_AUDIO_POSITION_TYPE, temp);

    setting.endGroup();
}

audio_default::n_audio_recod audio_default::load_default(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_POSITION);

    int temp = setting.value(KEY_AUDIO_POSITION_TYPE, audio_default::not_define).toInt();

    setting.endGroup();

    return static_cast<audio_default::n_audio_recod>(temp);

}

void audio_default_location::on_button_cancel_clicked()
{
    this->close();
}


void audio_default_location::on_button_ext_clicked()
{
    SET_CHECK(true)
}

void audio_default_location::on_button_into_clicked()
{
    SET_CHECK(false)
}
