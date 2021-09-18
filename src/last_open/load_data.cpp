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

bool last_file::load_data()
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    QByteArray array;
    const size_t size = sizeof(last_file_s);
    uchar data;
    uint i;
    size_t k;
    last_file_s file;

    setting.beginGroup(GROUPNAME_LAST_FILE);

    array = setting.value(KEY_LAST_BASE_FILE).toByteArray();
    setting.endGroup();

    const uint len = array.size() / size;

    for(i=0; i<len; ++i){
        for(k=0; k<size; ++k){
            data = array.at(i+k);

            size_t point = (size_t )&file + k;

            memcpy((void *)point, &data, sizeof(data));
        }
        m_data.append(file);
    }

    return true;
}
