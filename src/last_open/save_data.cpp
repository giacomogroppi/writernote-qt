#include "struct_last_file.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include "../utils/time/current_time.h"

#define TOCHAR toUtf8().constData()
#include "../utils/setting_define.h"
#include "../utils/remove_key/remove_key.h"

void save_data(QString &path, int type, int owner_type, char *owner)
{
    int quanti, i;
    bool uguale;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);

    setting.beginGroup(GROUPNAME_LAST_FILE);

    quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();

    last_file *m_lista;

    m_lista = load_data(setting, quanti);

    if(!m_lista){
        quanti = 0;
    }

    for(i=0, uguale = false; i<quanti && !uguale; i++){
        if((QString)m_lista[i].posizione ==  path){
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
        strncpy(m_lista[i].last_modification_o, time_now.TOCHAR, sizeof(char)*MAXMOD__FILE);
        strncpy(m_lista[i].last_modification_g, day_now.TOCHAR, sizeof(char)*MAXMOD__FILE);
    }
    else{
        quanti ++;
        last_file *temp_e = new last_file;
        strncpy(temp_e->last_modification_o, time_now.TOCHAR, sizeof(char)*MAXMOD__FILE);
        strncpy(temp_e->last_modification_g, day_now.TOCHAR, sizeof(char)*MAXMOD__FILE);

        strncpy(temp_e->posizione, path.TOCHAR, sizeof(char)*MAXSTR__FILE);

        temp_e->owner.type_user = owner_type;

        /* cloud */
        if(owner)
            strncpy(temp_e->posizione, owner, sizeof(char)*MAXSTR__FILE);

        temp_e->type = TYPE_COMPUTER;

        last_file *temp_file = new last_file[quanti];

        if(m_lista)
            memccpy(temp_file, m_lista, quanti-1, sizeof(last_file));

        memccpy(&temp_file[quanti-1], temp_e, 1, sizeof(last_file));

        save_data_f(setting, quanti, temp_file);

        delete [] temp_file;
        if(m_lista)
            delete m_lista;

        return;
    }

    save_data_f(setting, quanti, m_lista);

    if(m_lista)
        delete m_lista;

}

void save_data_f(QSettings &setting, int quanti, last_file *m_lista){
    setting.setValue(KEY_LAST_FILE_QUANTI, quanti);

    QByteArray array;

    array.append((const char *)m_lista, sizeof(last_file)*quanti);

    setting.setValue(KEY_LAST_BASE_FILE, array);

    setting.endGroup();
}
