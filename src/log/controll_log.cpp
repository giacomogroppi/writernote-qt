#include "controll_log.h"
#include <QString>
#include "../utils/setting_define.h"
#include <QSettings>
#include "../utils/path/get_path.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../utils/areyousure/areyousure.h"
#include "../datawrite/qfilechoose.h"
#include "../utils/path/get_path.h"
#include "../utils/permission/permission.h"
#include "../utils/time/current_time.h"

controll_log::controll_log(QObject *parent) : QObject(parent)
{
    if(loadData()){
        dialog_critic("We had a problem loading the path for log, do you want to set a position? ");


        QString _path;
        e_setOut res = setOut(_path);

        if(res == e_setOut::ok){
            this->m_permi = permi::enable;
        }else if(e_setOut::ref){
            this->m_permi = permi::disable;
            return;
        }


        if(pos.isEmpty()){
            this->m_permi = permi::error;
        }

    }

}

controll_log::~controll_log()
{
    saveData();
}

controll_log::e_setOut controll_log::setOut(QString &_path)
{
    bool temp = true;
    if(areyousure(nullptr, "Error saving log file", "We had a proble loading the path for log, do you wanto to set a new position?")){
        while(temp){
            if(!qfilechoose::filechoose(&_path, TYPELOG)){
                temp = false;
            }

            if(!permission::folderExist(_path.toUtf8().constData())){
                dialog_critic("The folder didn't exist");
            }
            else{
                temp = false;
                this->pos = _path;
            }
        }

    }
}

void controll_log::saveData(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);
    setting.setValue(KEY_LOG_POSITION, this->pos);

    setting.setValue(KEY_LOG_POSITION_DEFINE, this->m_permi);

    setting.endGroup();
}


int controll_log::loadData(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LOG_POSITION);

    this->m_permi = static_cast<permi>(setting.value(KEY_LOG_POSITION_DEFINE, permi::enable).toInt());

    if(m_permi == permi::disable)
        return 0;

    pos = setting.value(KEY_LOG_POSITION, "").toString();

    if(pos == ""){
        const char *_pos = get_path(path::audio_pos);
        if(!_pos)
            return 1;

        pos = _pos;
        pos += "/";
        pos += "writernote-log-" + current_day_string() + current_time_string() + ".txt";
    }

    setting.endGroup();
    return 0;
}
