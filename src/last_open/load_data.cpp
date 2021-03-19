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

last_file * load_data(int quanti)
{
    if(quanti == 0)
        return NULL;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);

    last_file *temp;

    auto array = setting.value(KEY_LAST_BASE_FILE).toByteArray();
    setting.endGroup();

    /* check the integrity of data */
    if(array.size() != sizeof(last_file)*quanti)
        return NULL;

    temp = (last_file *)array.data();

    last_file *temp_return;

    temp_return = (last_file *)malloc(sizeof(last_file)*quanti);

    memcpy(temp_return, temp, sizeof(last_file)*quanti);

    return temp_return;
}

/*
 * we can directly understand how many
 * files there are by dividing the
 * size of the qbytearray by its size.
 * as a matter of error control we save
 * its size with another key
*/
int load_quanti(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_FILE);
    bool temp;
    int res = setting.value(KEY_LAST_FILE_QUANTI, 0).toInt(&temp);

    setting.endGroup();

    return (temp) ? res: 0;

}
