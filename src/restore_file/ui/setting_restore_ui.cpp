#include "setting_restore_ui.h"
#include "ui_setting_restore_ui.h"
#include "../../utils/setting_define.h"
#include "../../indice_class.h"
#include "../../currenttitle/currenttitle_class.h"
#include "../../datawrite/savefile.h"
#include "../../utils/dialog_critic/dialog_critic.h"
#include "../../utils/slash/slash.h"

#include "../get_name_tmp.h"

#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QFile>

#define MAX_SAVE 3600
#define MIN_SAVE 1

extern bool need_save_auto;
extern bool need_save_tmp;

setting_restore_ui::setting_restore_ui(QWidget *parent,
                                       currenttitle_class **curr,
                                       indice_class *ind,
                                       QString *pp) :
    QDialog(parent),
    ui(new Ui::setting_restore_ui)
{
    ui->setupUi(this);

    m_first = new QTimer(this);
    m_sec = new QTimer(this);

    QObject::connect(m_first, &QTimer::timeout, this, &setting_restore_ui::firstTimer);
    QObject::connect(m_sec, &QTimer::timeout, this, &setting_restore_ui::secondTimer);

    ui->spinBox_autosave->setMaximum(MAX_SAVE);
    ui->spinBox_autosave->setMinimum(MIN_SAVE);

    ui->spinBox_temp->setMaximum(MAX_SAVE);
    ui->spinBox_temp->setMinimum(MIN_SAVE);

    ui->checkBox_autosave->setCheckable(true);
    ui->checkBox_temp->setCheckable(true);

    this->m_curr = curr;
    this->m_ind = ind;
    this->m_path = pp;

    loadData();

    startTimerSetting();

}

void setting_restore_ui::startTimerSetting(){
    m_first->stop();
    m_sec->stop();

    if(m_data.autosave){
        m_first->start(m_data.t_autosave*1000);
    }

    if(m_data.temp_file){
        m_sec->start(m_data.t_temp_file*1000);
    }

}

setting_restore_ui::~setting_restore_ui()
{
    //this->changeCopybookFile();

    saveData();
    delete ui;
}

void setting_restore_ui::deleteFile()
{
    QString ff = get_name_tmp::get(m_path);

    if(!QFile::remove(ff)){
        messaggio_utente("I had a problem removing the temp file in " + ff);
    }
}

void setting_restore_ui::updateWindow()
{
    ui->checkBox_autosave->setChecked(m_data.autosave);
    ui->spinBox_autosave->setValue(m_data.t_autosave);

    ui->checkBox_temp->setChecked(m_data.temp_file);
    ui->spinBox_temp->setValue(m_data.t_temp_file);
}

void setting_restore_ui::loadData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_INT);

    m_data.autosave = setting.value(KEY_INT_AUTOSAVE_ENABLE, false).toBool();
    m_data.t_autosave = setting.value(KEY_INT_AUTOSAVE_TIME, 5).toUInt();

    m_data.temp_file = setting.value(KEY_INT_TMP_ENABLE, true).toBool();
    m_data.t_temp_file = setting.value(KEY_INT_TMP_TIME, 5).toUInt();

    m_data.remove_file = setting.value(KEY_INT_REMOVE_FILE_ENABLE, true).toBool();

    setting.endGroup();

    this->updateWindow();
}

void setting_restore_ui::saveData()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_INT);

    setting.setValue(KEY_INT_AUTOSAVE_ENABLE, m_data.autosave);
    setting.setValue(KEY_INT_AUTOSAVE_TIME, m_data.t_autosave);

    setting.setValue(KEY_INT_TMP_ENABLE, m_data.temp_file);
    setting.setValue(KEY_INT_TMP_TIME, m_data.t_temp_file);

    setting.setValue(KEY_INT_REMOVE_FILE_ENABLE, m_data.remove_file);

    setting.endGroup();
}

void setting_restore_ui::firstTimer()
{
    savefile ff(m_path, *m_curr);
    int res;

    if(!need_save_auto){
        goto start_timer;
        return;
    }

    if((*m_curr)->nome_copybook == ""){
        /* it's not loaded */
        goto start_timer;
    }

    res = ff.savefile_check_file() == OK
            && ff.savefile_check_indice(m_ind);

    if(!res){
        dialog_critic("We had a problem saving the file in " + *m_path);
    }

    start_timer:
    need_save_auto = false;
    startTimerSetting();

}

static int try_save = 0;

void setting_restore_ui::secondTimer()
{
    int res;
    QString path;

    path = get_name_tmp::get(m_path);

    savefile ff(&path, *m_curr);

    if(!need_save_tmp)
        goto start_timer;

    if((*m_curr)->nome_copybook == ""){
        /* it's not loaded */
        goto start_timer;
    }

    res = ff.savefile_check_file() == OK
            && ff.savefile_check_indice(m_ind) == OK;

    if(!res){
        if(try_save > 5){
            dialog_critic("We had a problem saving the temporary file in " + path + " for " + QString::number(try_save));
        }
    }

    try_save = 0;

    start_timer:
    need_save_tmp = false;
    startTimerSetting();

}

void setting_restore_ui::on_pushButton_ok_clicked()
{
    emit dataChange();
    this->close();
}

void setting_restore_ui::on_pushButton_close_clicked()
{
    this->close();
}

void setting_restore_ui::on_spinBox_temp_valueChanged(int arg1)
{
    m_data.t_temp_file = arg1;
    if(m_data.t_temp_file > m_data.t_autosave){
        m_data.t_autosave = m_data.t_temp_file;
    }

    updateWindow();
}

void setting_restore_ui::on_spinBox_autosave_valueChanged(int arg1)
{
    m_data.t_autosave = arg1;
    if(m_data.t_autosave < m_data.t_temp_file){
        m_data.t_temp_file = m_data.t_autosave;
    }

    updateWindow();
}

void setting_restore_ui::on_checkBox_autosave_stateChanged(int arg1)
{
    /* arg1 == 0 the user disable */

    m_data.autosave = arg1;
}

void setting_restore_ui::on_checkBox_temp_stateChanged(int arg1)
{
    m_data.temp_file = arg1;
}

void setting_restore_ui::on_checkBox_remove_stateChanged(int arg1)
{
    m_data.remove_file = arg1;
}
