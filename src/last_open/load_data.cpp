#include "struct_last_file.h"
#include "../utils/setting_define.h"

#include <QBitArray>

/*
 * data return from array.data() is released
 * after the function end
*/

last_file * load_data(QSettings &setting, int quanti)
{
    if(quanti == 0)
        return NULL;

    last_file *temp;

    QByteArray array;
    array = setting.value(KAY_LAST_BASE_FILE).toByteArray();

    /* check the integrity of data */
    if(array.size() != sizeof(last_file)*quanti)
        return NULL;

    temp = (last_file *)array.data();

    last_file *temp_return;
    temp_return = new last_file[quanti];

    memccpy(temp_return, temp, quanti, sizeof(last_file));

    return temp_return;
}
