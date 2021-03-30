#include "log_ui.h"
#include "ui_log_ui.h"

#include <QSettings>
#include "../../utils/setting_define.h"
#include "../../utils/path/get_path.h"

#include "../../utils/time/current_time.h"
#include "../../utils/mostra_explorer/mostra_explorer.h"
#include "../../utils/dialog_critic/dialog_critic.h"
#include "../../utils/slash/slash.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>

log_ui::log_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::log_ui)
{
    ui->setupUi(this);

    this->loadData();

}

log_ui::~log_ui()
{
    delete ui;

    this->saveData();
}

void log_ui::showAll()
{
    FILE *fp;
    fp = fopen(this->pos_log.toUtf8().constData(), "r");

    if(!fp){
        QString temp = ("Error open file" + pos_log).toUtf8().constData();
        this->ui->text_error_show->setText(temp);
        return;
    }




}

void log_ui::write(QString stringa, log_ui::type_write var)
{
    if(m_permi != permi::enable)
        return;

    FILE *fp;
    const char * stringa_;

    if(var == log_ui::info)
        stringa = "info: " + stringa;
    else if(var == log_ui::error_internal)
        stringa = "internal error: " + stringa;
    else
        stringa = "CRITIC ERROR: " + stringa;

    fp = fopen(this->pos_log.toUtf8().constData(), "a");

    if(!fp){
        this->ui->text_error_show->setText("Unable to save data");
        this->m_permi = permi::error;
        return;
    }

    stringa_ = stringa.toUtf8().constData();
    fprintf(fp, "%s", stringa_);

    fclose(fp);

}

QString log_ui::getNameLog()
{
    QString temp;
    temp = pos_log + slash::__slash() + current_day_string() + current_time_string();

    return temp;
}

void log_ui::closeEvent(QCloseEvent *event)
{
    /* we don't want to close actualy */
    this->close();

    event->ignore();
}

void log_ui::saveData(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);
    setting.setValue(KEY_LOG_POSITION, this->pos_log);

    setting.setValue(KEY_LOG_POSITION_DEFINE, this->m_permi);

    setting.endGroup();
}

int log_ui::loadData(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);

    const char *_pos;

    this->m_permi = static_cast<permi>(setting.value(KEY_LOG_POSITION_DEFINE, permi::enable).toInt());

    if(m_permi == permi::disable)
        return 0;

    pos_log = setting.value(KEY_LOG_POSITION, "").toString();

    if(pos_log == ""){
        _pos = get_path(path::audio_pos);
        if(!_pos)
            return 1;

        pos_log = _pos;
        pos_log += "/";
        pos_log += "writernote-log-" + current_day_string() + current_time_string() + ".txt";
    }

    setting.endGroup();
    return 0;
}



void log_ui::updateAll()
{
    FILE *fp;
    QString _text_log;
    char c;

    if(m_permi == permi::enable)
        ui->button_enable_log->setText("Disable");
    else
        ui->button_enable_log->setText("Enable");

    fp = fopen(pos_log.toUtf8().constData(), "r");

    if(!fp){
        ui->textBrowser->setText("No log available");
    }else{
        fscanf(fp, "%c", &c);
        while(!feof(fp)){
            _text_log.append(c);

            fscanf(fp, "%c", &c);
        }
        fclose(fp);

        ui->textBrowser->setText(_text_log);
    }

    updateError();


}

void log_ui::updateError()
{
    bool __need_hide = false;
    if(permi::error){
        __need_hide = true;

        ui->text_error_show->setText("We had a problem write text into " + pos_log);
    }

    ui->text_error_show->setVisible(__need_hide);

}

void log_ui::on_button_close_clicked()
{
    if(m_permi == permi::error){
        m_permi = permi::disable;
    }

    this->hide();
}

void log_ui::on_button_enable_log_clicked()
{

}

void log_ui::on_Button_open_log_clicked()
{
    mostra_explorer(pos_log);
}

void log_ui::on_button_change_position_clicked()
{
    QString __temp_dir;

    FILE *fp;

    __temp_dir = QFileDialog::getExistingDirectory();

    if(__temp_dir == ""){
        return;
    }

    fp = fopen((__temp_dir + "/" + "Temp.txt").toUtf8().constData(), "w");
    if(!fp){
        dialog_critic("Impossibile write a file into that position");
        return;
    }

    m_permi = permi::enable;
    this->pos_log =



}
