#include "struct_last_file.h"
#include "../utils/setting_define.h"
#include "../utils/get_file_dir/get_file_dir.h"
#include <QBitArray>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include "../utils/lastModification/lastmodification.h"
#include "../utils/removenotwriternote.h"
#include "../utils/common_def.h"
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
    uint i;
    last_file_s file;

    setting.beginGroup(GROUPNAME_LAST_FILE);

    array = setting.value(KEY_LAST_BASE_FILE).toByteArray();
    setting.endGroup();

    const uint len = array.size() / size;

    for(i=0; i<len; ++i){
        const char *data = array.mid(i*size, size*(i+1)).constData();
        memcpy(&file, data, size);
        m_data.append(file);
    }

    return true;
}

bool last_file::load_folder(const QString &path)
{
    const QStringList file = get_file_dir::get(path);
    const uint len = file.length();
    uint i;
    last_file_s object;

    for(i=0; i<len; ++i){
        const QString &ref = file.at(i);

        if(ref.indexOf('.' + APP_EXT) == -1)
            continue;

        strncpy(object.last_modification_g, lastModification::day(ref).toUtf8().constData(), MAXMOD__FILE);
        strncpy(object.last_modification_o, lastModification::hour(ref).toUtf8().constData(), MAXMOD__FILE);

        strncpy(object.posizione, ref.toUtf8().constData(), MAXSTR__FILE);
        object.owner.type_user = 0;
        object.type = TYPE_COMPUTER;

        this->m_data.append(object);
    }

    return this->m_data.length();
}
