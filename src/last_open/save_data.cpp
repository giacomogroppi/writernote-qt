#include "struct_last_file.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include "../utils/time/current_time.h"

#define TOCHAR(x) x.toUtf8().constData()
#include "../utils/setting_define.h"
#include "../utils/remove_key/remove_key.h"

#include <QDebug>
#include <QFile>

static void setData(last_file *, const QString & time_now, const QString & day_now);
static void setPosition(last_file * data, const QString &pos);

void save_data(const QString &path, int type, int owner_type, char *owner)
{
    Q_UNUSED(type);
    if(path.isEmpty())
        return;

    int quanti, i;
    bool uguale;
    QList<last_file> m_lista;
    QString owner_string;

    const QString time_now = current_time_string();
    const QString day_now = current_day_string();


    quanti = load_quanti();

    if(!load_data(quanti, m_lista))
        quanti = 0;

    for(i=0, uguale = false; i<quanti; ++i){
        if(strcmp(m_lista.at(i).posizione, path.toUtf8().constData()) == 0){
            uguale = true;
            break;
        }
    }

    if(uguale){
        setData(&m_lista.operator[](i), time_now, day_now);
    }
    else{
        last_file temp_append;
        QByteArray array;
        quanti ++;

        setData(&temp_append, time_now, day_now);

        setPosition(&temp_append, path);

        temp_append.owner.type_user = owner_type;

        /* cloud */
        if(owner){
            owner_string = owner;
            setPosition(&temp_append, owner_string);
        }

        temp_append.type = TYPE_COMPUTER;

        for(int i=0; i<m_lista.length()-1; ++i){
            array.append((const char *)&m_lista.at(i), sizeof(last_file));
        }

        /*if(m_lista)
            array.append((const char *)m_lista, sizeof(last_file)*(quanti-1));*/

        array.append((const char *)&temp_append, sizeof(last_file));

        save_quanti(quanti);
        save_data_f(array);

        return;
    }

    save_data_f(quanti, m_lista);

}

void save_quanti(int quanti){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    setting.setValue(KEY_LAST_FILE_QUANTI, quanti);
    setting.endGroup();
}

void save_data_f(int quanti, const QList<last_file> &m_lista){
    save_quanti(quanti);

    int i;
    QByteArray array;

    for(i=0; i<m_lista.length(); ++i){
        array.append((const char *)&m_lista.at(i), sizeof(last_file));
    }

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
void save_data_f(const QByteArray &array){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    setting.setValue(KEY_LAST_BASE_FILE, array);
    setting.endGroup();
}

static void setData(last_file *data, const QString & time_now, const QString & day_now){
    strncpy(data->last_modification_g, TOCHAR(day_now), sizeof(data->last_modification_g));
    strncpy(data->last_modification_o, TOCHAR(time_now), sizeof(data->last_modification_o));
}

static void setPosition(last_file * data, const QString &pos){
    strncpy(data->posizione, TOCHAR(pos), sizeof(data->posizione));
}

