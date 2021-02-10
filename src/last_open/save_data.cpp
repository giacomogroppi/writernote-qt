#include "save_data.h"

#include <QSettings>
#include "../utils/setting_define.h"
#include "load_data.h"
#include "../utils/time/current_time.h"

#include "../utils/setting_define.h"

static void save_data_f(QSettings &, int quanti, last_file * m_lista);

void save_data(QString &path, int type)
{
    Q_UNUSED(type)

    int quanti, i;
    bool uguale;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);

    quanti = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt();

    last_file *m_lista = load_data(setting);

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

    if(uguale){
        if((QString)m_lista[i].last_modification != time_now)
            strcpy(m_lista[i].last_modification, time_now.toUtf8().constData());
    }
    else{
        quanti += 1;
        last_file *temp_e = new last_file;
        strcpy(temp_e->last_modification, time_now.toUtf8().constData());
        strcpy(temp_e->posizione, path.toUtf8().constData());

        temp_e->type = TYPE_COMPUTER;

        last_file *temp_file = new last_file[quanti];

        int i;
        for(i=0; i<quanti; i++){
            memccpy(temp_file, m_lista, quanti, sizeof(last_file));
        }

    }

    save_data_f(setting, quanti, m_lista);
}

static void save_data_f(QSettings &setting, int quanti, last_file *m_lista){
    setting.setValue(KEY_LAST_FILE_QUANTI, quanti);

    QByteArray array;

    array.append((const char *)m_lista, sizeof(last_file)*quanti);

    setting.setValue(KAY_LAST_BASE_FILE, array);

    setting.endGroup();
}
