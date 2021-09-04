#include "struct_last_file.h"
#include "../utils/setting_define.h"

#include <QBitArray>

/*
 * data return from array.data() is released
 * after the function end
 *
 * the function return a pointer to an last_file
 * in size of [quanti] allocate with malloc
 *
 * the memory is release when it's close last_open instance
*/

bool load_data(const int quanti, QList<last_file> &ret)
{
    if(quanti == 0)
        return false;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    QByteArray array;
    //last_file *temp;
    const size_t size = sizeof(last_file);
    uchar data;
    int i;
    size_t k;
    last_file file;

    setting.beginGroup(GROUPNAME_LAST_FILE);

    array = setting.value(KEY_LAST_BASE_FILE).toByteArray();
    setting.endGroup();

    /* check the integrity of data */
    if(array.size() != sizeof(last_file)*quanti)
        return false;

    for(i=0; i<quanti; ++i){
        for(k=0; k<size; ++k){
            data = array.at(i+k);

            size_t point = (size_t )&file + k;

            memcpy((void *)point, &data, sizeof(data));
        }
        ret.append(file);
    }

    return true;
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
