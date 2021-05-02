#include "struct_last_file.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include "../utils/time/current_time.h"

#define TOCHAR(x) x.toUtf8().constData()
#include "../utils/setting_define.h"
#include "../utils/remove_key/remove_key.h"

#include <QDebug>
#include <QFile>

static void setData(last_file *, QString & time_now, QString & day_now);
static void setPosition(last_file * data, QString &pos);

void save_data(QString &path, int type, int owner_type, char *owner)
{
    if(path.isEmpty())
        return;

    int quanti, i;
    bool uguale;

    quanti = load_quanti();

    last_file * m_lista = load_data(quanti);

    if(!m_lista){
        quanti = 0;
    }

    for(i=0, uguale = false; i<quanti && !uguale; i++){
        if(strcmp(m_lista[i].posizione, TOCHAR(path)) == 0){
            uguale = true;
        }
    }

    /*
     * it means there is already a recent file
     * name like path
    */
    QString time_now = current_time_string();
    QString day_now = current_day_string();

    if(uguale){
        setData(&m_lista[i], time_now, day_now);
    }
    else{
        quanti ++;
        last_file temp_append;

        setData(&temp_append, time_now, day_now);

        setPosition(&temp_append, path);

        temp_append.owner.type_user = owner_type;

        /* cloud */
        if(owner){
            qDebug("Change position because it's owner is external ");
            QString owner_string = owner;
            setPosition(&temp_append, owner_string);
        }

        temp_append.type = TYPE_COMPUTER;

        QByteArray array ;

        if(m_lista)
            array.append((const char *)m_lista, sizeof(last_file)*(quanti-1));

        array.append((const char *)&temp_append, sizeof(last_file));

        save_quanti(quanti);
        save_data_f(array);

        if(m_lista)
            free(m_lista);

        return;
    }

    save_data_f(quanti, m_lista);

    if(m_lista)
        free(m_lista);

}

void save_quanti(int quanti){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    setting.setValue(KEY_LAST_FILE_QUANTI, quanti);
    setting.endGroup();
}

void save_data_f(int quanti, last_file *m_lista){
    save_quanti(quanti);

    QByteArray array;

    array.append((const char *)m_lista, sizeof(last_file)*quanti);

    save_data_f(array);
}


/*
 * I don't know why, and apparently it's a bug,
 * but when we save a dynamic array, which in
 * this case is a qbytearray, we automatically
 * lose some data, or in any case we have some
 * wrong data, even if we read it immediately.
 * so for now it is not disabled.
 *
 * TODO -> implement a method in which only
 * strings and numbers are saved, and not structs
*/
void save_data_f(QByteArray &array){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    setting.setValue(KEY_LAST_BASE_FILE, array);
    setting.endGroup();
}

static void setData(last_file *data, QString & time_now, QString & day_now){
    strncpy(data->last_modification_g, TOCHAR(day_now), sizeof(data->last_modification_g));
    strncpy(data->last_modification_o, TOCHAR(time_now), sizeof(data->last_modification_o));
}

static void setPosition(last_file * data, QString &pos){
    strncpy(data->posizione, TOCHAR(pos), sizeof(data->posizione));
}

