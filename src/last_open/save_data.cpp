#include "struct_last_file.h"
#include <QSettings>
#include "../utils/setting_define.h"
#include "../utils/time/current_time.h"
#define TOCHAR(x) x.toUtf8().constData()
#include "../utils/setting_define.h"
#include "../utils/remove_key/remove_key.h"
#include <QDebug>
#include <QFile>

static void setData(last_file_s *, const QString & time_now, const QString & day_now);
static void setPosition(last_file_s * data, const QString &pos);

void last_file::save_data(const QString &path, int type, int owner_type, const char *owner)
{
    Q_UNUSED(type);
    if(path.isEmpty())
        return;

    last_file object;
    QString owner_string;

    const QString time_now = current_time_string();
    const QString day_now = current_day_string();

    object.load_data();


    last_file_s tmp_append;
    setData(&tmp_append, time_now, day_now);
    setPosition(&tmp_append, path);
    tmp_append.owner.type_user = owner_type;
    /* cloud */
    if(owner){
        owner_string = owner;
        setPosition(&tmp_append, owner_string);
    }
    tmp_append.type = TYPE_COMPUTER;

    object.m_data.append(tmp_append);
    object.save_data_setting();
}



void last_file::removeDouble(QList<last_file_s> &file)
{
    int i, k;
    for(i=0; i<file.length(); ++i){
        for(k=i; k<file.length(); ++k){
            if((QString)file.at(i).posizione == (QString)file.at(k).posizione && i != k){
                file.removeAt(k);
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
    uint i, k;
    QString first, second;

    for(i=0; i<this->length()-1; ++i){
        for(k=i+1; k<this->length(); ++k){
            first = at(i).last_modification_g + (QString)at(i).last_modification_o;
            second = at(k).last_modification_g + (QString)at(i).last_modification_o;

            REMOVE_CHAR(first, second, ':');

            if(atoi(first.toUtf8().constData()) < atoi(second.toUtf8().constData())){
                dochange(at_mod(i), at_mod(k));
            }

        }
    }
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

    for(i=0; i<m_data.length(); ++i){
        array.append((const char *)&m_data.at(i), sizeof(last_file_s));
    }

    save_data_setting(array);
}

static void setData(last_file_s *data, const QString & time_now, const QString & day_now){
    strncpy(data->last_modification_g, TOCHAR(day_now), sizeof(data->last_modification_g));
    strncpy(data->last_modification_o, TOCHAR(time_now), sizeof(data->last_modification_o));
}

static void setPosition(last_file_s * data, const QString &pos){
    strncpy(data->posizione, TOCHAR(pos), sizeof(data->posizione));
}

