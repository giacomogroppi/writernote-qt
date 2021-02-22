#include "struct_last_file.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include "../utils/time/current_time.h"

#include "../utils/setting_define.h"

void save_data(QString &path, int type, int owner_type, char *owner)
{
    int quanti, i;
    bool uguale;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);

    setting.beginGroup(GROUPNAME_LAST_FILE);

    quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();

    last_file *m_lista = NULL;

    m_lista = load_data(setting, quanti);

    for(i=0, uguale = false; i<quanti && !uguale; i++){
        if((QString)m_lista[i].posizione ==  path.toUtf8().constData()){
            uguale = true;
        }
    }

    /*
     * it means there is already a recent file
     * name like path
    */
    QString time_now = current_time_string();
    QString day_now = current_day_string();

#define TOCHAR toUtf8().constData()

    if(uguale){
        strcpy(m_lista[i].last_modification_o, time_now.TOCHAR);
        strcpy(m_lista[i].last_modification_g, day_now.TOCHAR);
    }
    else{
        quanti ++;
        last_file *temp_e = new last_file;
        strcpy(temp_e->last_modification_o, time_now.TOCHAR);
        strcpy(temp_e->last_modification_g, day_now.TOCHAR);

        strcpy(temp_e->posizione, path.toUtf8().constData());

        temp_e->owner.type_user = owner_type;

        if(owner)
            strcpy(temp_e->posizione, owner);

        temp_e->type = TYPE_COMPUTER;

        last_file *temp_file = new last_file[quanti];

        if(m_lista)
            memccpy(temp_file, m_lista, quanti-1, sizeof(last_file));

        memccpy(&temp_file[quanti-1], temp_e, 1, sizeof(last_file));

        return save_data_f(setting, quanti, temp_file);
    }

    save_data_f(setting, quanti, m_lista);

    if(m_lista)
        delete m_lista;
}

void save_data_f(QSettings &setting, int quanti, last_file *m_lista){
    setting.setValue(KEY_LAST_FILE_QUANTI, quanti);

    QByteArray array;

    array.append((const char *)m_lista, sizeof(last_file)*quanti);

    setting.setValue(KAY_LAST_BASE_FILE, array);

    setting.endGroup();
}
