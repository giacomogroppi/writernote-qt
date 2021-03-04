#include "struct_last_file.h"
#include "../utils/setting_define.h"

#include <QBitArray>

/*
 * data return from array.data() is released
 * after the function end
 *
 * the function return a pointer to an last_file
 * in size of [quanti]
 *
 * the memory is release when it's close last_open instance
*/

last_file * load_data(QSettings &setting, int quanti)
{
    if(quanti == 0)
        return NULL;

    last_file *temp;

    QByteArray array;
    array = setting.value(KEY_LAST_BASE_FILE).toByteArray();


    /* check the integrity of data */
    if(array.size() != sizeof(last_file)*quanti)
        return NULL;

    temp = (last_file *)array.data();

    last_file *temp_return;

    temp_return = new last_file[quanti];

#if defined(WIN32) || defined(WIN64)
    for(int i=0; i<quanti; i++){
        memcpy(&temp_return[i], &temp[i], sizeof(last_file));
    }
#elif defined(unix)
    memccpy(temp_return, temp, quanti, sizeof(last_file));
#endif
    return temp_return;
}
