#include "load_data.h"
#include "../utils/setting_define.h"

QList<last_file *> load_data(QSettings &setting, int quanti)
{
    int i;

    QList<last_file *> m_lista;

    last_file *temp;

    for(i=0; i<quanti; i++){
        QByteArray array;
        array = setting.value((QString)KAY_BASE_FILE + QString::number(i)).toByteArray();

        temp = (last_file *)array.data();

        m_lista.append(temp);
    }

    return m_lista;
}
