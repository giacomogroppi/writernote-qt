#include "struct_last_file.h"
#include <QSettings>
#include "../utils/setting_define.h"
#include "../utils/time/current_time.h"
#include "../utils/setting_define.h"
#include "../utils/remove_key/remove_key.h"
#include <QDebug>
#include <QFile>

static void setString(char * data, const QString &pos);

void last_file::save_data(const QString &path, int type, int owner_type, const char *owner)
{
    Q_UNUSED(type);
    if(path.isEmpty())
        return;

    last_file object;
    QString owner_string;
    last_file_s tmp_append;
    const QString time_now = current_time_string();
    const QString day_now = current_day_string();

    object.load_data();

    setString(tmp_append.last_modification_g, day_now);
    setString(tmp_append.last_modification_o, time_now);
    setString(tmp_append.posizione, path);
    tmp_append.owner.type_user = owner_type;
    /* cloud */
    if(owner){
        owner_string = owner;
        setString(tmp_append.owner.name, owner_string);
    }
    tmp_append.type = TYPE_COMPUTER;

    object.m_data.append(tmp_append);
    object.removeDouble();

    object.save_data_setting();
}

void last_file::removeDouble()
{
    int i, k;
    for(i=0; i<m_data.length(); ++i){
        for(k=i; k<m_data.length(); ++k){
            if((QString)m_data.at(i).posizione == (QString)m_data.at(k).posizione && i != k){
                m_data.removeAt(k);
            }
        }
    }
}

/*
 * la funzione riordina i file dal
 * più recente al più vecchio
 * In caso ci siano più copybook
 * del numero settato dall'utente,
 * li alimina automaticamente
 *
 * the function reorders files
 * from newest to oldest
*/
#define REMOVE_CHAR(str, str2, c) str.remove(c); \
    str2.remove(c)

static void dochange(last_file_s &first, last_file_s &second){
    last_file_s tmp;

    memcpy(&tmp, &first, sizeof(tmp));
    memcpy(&first, &second, sizeof(tmp));
    memcpy(&second, &tmp, sizeof(tmp));
}

void last_file::tidyup()
{
    int i, k;
    QString first, second;
    int len;

    len = length();

    for(i=0; i < (len-1); ++i){
        for(k=i+1; k<len; ++k){
            first = at(i).last_modification_g + (QString)at(i).last_modification_o;
            second = at(k).last_modification_g + (QString)at(i).last_modification_o;

            REMOVE_CHAR(first, second, ':');

            if(atoi(first.toUtf8().constData()) < atoi(second.toUtf8().constData())){
                dochange(at_mod(i), at_mod(k));
            }

        }
    }

    this->removeDouble();
}


void last_file::save_data_setting(const QByteArray &array){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    setting.setValue(KEY_LAST_BASE_FILE, array);
    setting.endGroup();
}

void last_file::save_data_setting(){
    int i;
    QByteArray array;
    const char *data;

    for(i=0; i<m_data.length(); ++i){
        data = (const char *)&m_data.at(i);
        array.append(data, sizeof(last_file_s));
    }

    save_data_setting(array);
}

static void setString(char * data, const QString &pos){
    int i;
    const int len = pos.length();

    for(i=0; i<len; ++i){
        data[i] = pos.at(i).toLatin1();
    }
    data[i] = '\0';
}

