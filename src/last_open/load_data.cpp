#include "struct_last_file.h"
#include "../utils/setting_define.h"

#include <QBitArray>

last_file * load_data(QSettings &setting)
{
    last_file *temp;

    QByteArray array;
    array = setting.value(KAY_LAST_BASE_FILE).toByteArray();

    temp = (last_file *)array.data();


    return temp;
}
