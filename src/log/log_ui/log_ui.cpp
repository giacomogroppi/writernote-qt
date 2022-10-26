#include "log_ui.h"
#include "ui_log_ui.h"
#include <QSettings>
#include "utils/setting_define.h"
#include "utils/path/get_path.h"
#include "utils/time/current_time.h"
#include "utils/mostra_explorer/mostra_explorer.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/slash/slash.h"
#include "utils/time/current_time.h"
#include "utils/WCommonScript.h"
#include "core/WFile.h"
#include "core/WMutex.h"
#include "core/WMutexLocker.h"

#include <pthread.h>
#include <QDir>
#include <QFile>
#include <QFileDialog>

static WMutex mutex;

log_ui::log_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::log_ui)
{
    ui->setupUi(this);

    this->loadData();
    this->hide();
}

log_ui::~log_ui()
{
    this->saveData();
    delete ui;
}

bool log_ui::getData(QByteArray &data)
{
    if(WFile::readFile(data, pos_log.toUtf8().constData()) < 0)
        return false;
    return true;
}

QString log_ui::adjust_path(const QString &str) const
{
    QString tmp = str;

    tmp.replace(QChar(':'), QChar('_'));
    tmp.replace(QChar(' '), QChar('_'));

    return tmp;
}

bool log_ui::check_str(const QString &str) const
{
    const auto res = str.indexOf(":");
    if(res < 0)
        return true;
    return false;
}

void log_ui::showAll()
{
    QString err;
    QByteArray data;
    if(!getData(data)){
        err = QString("Error open file %1").arg(pos_log);
        this->ui->text_error_show->setText(err);
    }

    ui->textBrowser->setText(data);
}

void log_ui::write(const QString &stringa, log_ui::type_write var)
{
    QFile file(pos_log.toUtf8().constData());
    QString tmp;

    if(m_permi != permi::enable){
        qDebug() << "It's not possibile write to log because of permission";
        qDebug() << "Message: " << stringa;
    }

    if(var == log_ui::info)
        tmp = "info: " + stringa;
    else if(var == log_ui::error_internal)
        tmp = "internal error: " + stringa;
    else
        tmp = "CRITIC ERROR: " + stringa;

    WMutexLocker _(mutex);

    if(unlikely(!file.open(QIODevice::Append))){
        ui->text_error_show->setText("Unable to save data");
        m_permi = permi::error;
        return;
    }

    log_ui::addTime(tmp);

    file.write(QString("\n%1 --- %2").arg(tmp).arg(stringa).toUtf8());

    file.close();
}

void log_ui::print(FILE *fp, const QByteArray &str)
{
    fprintf(fp, "%s", str.constData());
}

void log_ui::addTime(QString &message)
{
    message = current_day_string() + current_time_string() + message;
}

QString log_ui::getNameLog()
{
    QString temp;
    temp = pos_log + slash::__slash() +
            adjust_path(current_day_string()) +
            adjust_path(current_time_string());

    return temp;
}

void log_ui::closeEvent(QCloseEvent *event)
{
    /* we don't want to close actualy */
    this->hide();

    event->ignore();
}

void log_ui::saveData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);
    setting.setValue(KEY_LOG_POSITION, this->pos_log);

    setting.setValue(KEY_LOG_POSITION_DEFINE, this->m_permi);

    setting.endGroup();
}


int log_ui::loadData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);

    QString _pos;

    this->m_permi = static_cast<permi>(setting.value(KEY_LOG_POSITION_DEFINE, permi::enable).toInt());

    if(m_permi == permi::disable)
        return 0;

    pos_log = setting.value(KEY_LOG_POSITION, "").toString();

    if (!check_str(pos_log)) {
        pos_log = "";
    }

    if(pos_log.isEmpty()){
        _pos = get_path(path::audio_pos);
        if(_pos == ""){
            setting.endGroup();
            return 1;
        }

        pos_log = _pos;
        pos_log += slash::__slash();
        pos_log += QString("writernote-log-%1-%2.txt").
                arg(adjust_path(current_day_string())).
                arg(adjust_path(current_time_string()));
    }

    setting.endGroup();
    return 0;
}



void log_ui::updateAll()
{
    FILE *fp;
    QString _text_log;
    char c;
    char error = 0;

    if(m_permi == permi::enable)
        ui->button_enable_log->setText("Disable");
    else
        ui->button_enable_log->setText("Enable");
#if defined(unix) || defined(MACOS)
    fp = fopen(pos_log.toUtf8().constData(), "r");
#elif defined(WIN32) || defined (WIN64)
    fp = fopen(pos_log.toUtf8().constData(), "rb");
#endif

    if(!fp){
        ui->textBrowser->setText("No log available");
    }else{
        if(!fscanf(fp, "%c", &c)) 
            error = 1;
        while(!feof(fp)){
            _text_log.append(c);

            if(!fscanf(fp, "%c", &c))
                error = 1;
        }

        if(error)
            _text_log += "Error reading log file in some byte";

        ui->textBrowser->setText(_text_log);
    }

    fclose(fp);

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

    fp = fopen((__temp_dir + slash::__slash() + "Temp.txt").toUtf8().constData(), "w");
    if(!fp){
        dialog_critic("Impossibile write a file into that position");
        return;
    }

    m_permi = permi::enable;
    this->pos_log = getNameLog();

}

log_ui * NAME_LOG_EXT;
