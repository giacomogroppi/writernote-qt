#include "struct_user.h"

#include "../utils/setting_define.h"
#include <QSettings>

#include "stdio.h"
#include "stdlib.h"
#include <QDataStream>

#ifdef CLOUD

void save_recent_user(struct struct_user *data){
    QSettings setting(ORGANIZATIONAME, FILE_NAME_USER_CLOUD);
    setting.beginGroup(GROUPNAME_USER_CLOUD);


    setting.setValue(GROUPNAME_USER_CLOUD_IS_DEFINED, true);

    QByteArray array;
    array.setRawData((const char *)data, sizeof(struct struct_user));

    setting.endGroup();

}

/*
 * after calling this function you need
 * to free up the memory at the end
*/
struct struct_user * load_recent_user(){
    struct struct_user *user = NULL;

    QSettings setting(ORGANIZATIONAME, FILE_NAME_USER_CLOUD);
    if(!setting.value(GROUPNAME_USER_CLOUD_IS_DEFINED, false).toBool())
        return user;


    QByteArray array;
    QByteArray default_loading;
    default_loading.append((QString)NOT_USER_RECENT);

    array = setting.value(GROUPNAME_USER_CLOUD, default_loading).toByteArray();

    if(array.toStdString() == std::to_string(NOT_USER_RECENT))
        return NULL;


    /* memory of array.data() is automaticaly relised after function end */
    void *temp = array.data();

    user = new struct struct_user;

    memcpy(user, temp, sizeof(struct struct_user));

    return user;

}
#endif //CLOUD
