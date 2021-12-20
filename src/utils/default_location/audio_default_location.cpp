#include "audio_default_location.h"
#include "ui_audio_default_location.h"

#include <QSettings>
#include "utils/setting_define.h"

#define SET_CHECK(x) ui->button_into->setChecked((x)); \
    ui->button_ext->setChecked(!(x));


audio_default_location::audio_default_location(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::audio_default_location)
{
    ui->setupUi(this);

    m_lastchoise = audio_default_location::load_default();

    this->ui->button_ext->setCheckable(true);
    this->ui->button_into->setCheckable(true);


    if(m_lastchoise == audio_default_location::not_define){
        this->ui->button_ext->setChecked(false);
        this->ui->button_into->setChecked(false);

        return;
    }

    SET_CHECK(m_lastchoise);

}

audio_default_location::~audio_default_location()
{
    /* he didn't save */

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

    int temp = this->m_lastchoise;

    setting.setValue(KEY_AUDIO_POSITION_TYPE, temp);

    setting.endGroup();
}

audio_default_location::n_audio_record audio_default_location::load_default(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUDIO_POSITION);

    int temp = setting.value(KEY_AUDIO_POSITION_TYPE, n_audio_record::not_define).toInt();

    setting.endGroup();

    return static_cast<audio_default_location::n_audio_record>(temp);

}

void audio_default_location::on_button_cancel_clicked()
{
    this->close();
}


void audio_default_location::on_button_ext_clicked()
{
    this->m_lastchoise = n_audio_record::external;
    SET_CHECK(true)
}

void audio_default_location::on_button_into_clicked()
{
    this->m_lastchoise = n_audio_record::internal;

    SET_CHECK(false)
}
