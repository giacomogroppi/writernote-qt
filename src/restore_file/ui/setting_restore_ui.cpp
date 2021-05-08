#include "setting_restore_ui.h"
#include "ui_setting_restore_ui.h"
#include "../../utils/setting_define.h"
#include "../../indice_class.h"
#include "../../currenttitle/currenttitle_class.h"

#include <QSettings>
#include <QTimer>
#include <QDebug>

#define MAX_SAVE 3600
#define MIN_SAVE 1

setting_restore_ui::setting_restore_ui(QWidget *parent,
                                       currenttitle_class **curr,
                                       indice_class *ind) :
    QDialog(parent),
    ui(new Ui::setting_restore_ui)
{
    ui->setupUi(this);

    ui->spinBox_autosave->setMaximum(MAX_SAVE);
    ui->spinBox_autosave->setMinimum(MIN_SAVE);

    ui->spinBox_temp->setMaximum(MAX_SAVE);
    ui->spinBox_temp->setMinimum(MIN_SAVE);


    ui->checkBox_autosave->setCheckable(true);
    ui->checkBox_temp->setCheckable(true);

    this->m_curr = curr;
    this->m_ind = ind;

    loadData();

}

setting_restore_ui::~setting_restore_ui()
{
    saveData();
    delete ui;
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


    setting.endGroup();
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
