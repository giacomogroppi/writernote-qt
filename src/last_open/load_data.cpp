#include "load_data.h"
#include "../utils/setting_define.h"

last_file * load_data(QSettings &setting)
{
    last_file *temp;

    QByteArray array;
    array = setting.value(KAY_LAST_BASE_FILE).toByteArray();

    temp = (last_file *)array.data();



    return temp;
}
