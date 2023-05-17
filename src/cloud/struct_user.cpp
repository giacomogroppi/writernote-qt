#include "struct_user.h"
#include "utils/setting_define.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils/remove_key/remove_key.h"

#ifdef CLOUD

void save_recent_user(struct struct_user *data){
    QSettings setting(ORGANIZATIONAME, FILE_NAME_USER_CLOUD);
    setting.beginGroup(GROUPNAME_CLOUD_USER);


    setting.setValue(KEY_USER_CLOUD_IS_DEFINED, true);

    WByteArray array;
    array.setRawData((const char *)data, sizeof(struct struct_user));

    setting.setValue(KEY_USER_CLOUD_STRUCT, array);

    setting.endGroup();

}

/*
 * after calling this function you need
 * to free up the memory at the end
*/
struct struct_user * load_recent_user(){
    struct struct_user *user = NULL;

    QSettings setting(ORGANIZATIONAME, FILE_NAME_USER_CLOUD);
    setting.beginGroup(GROUPNAME_CLOUD_USER);

    if(!setting.value(KEY_USER_CLOUD_IS_DEFINED, false).toBool()){
        setting.endGroup();
        return user; /*NULL*/
    }


    WByteArray array;
    WByteArray default_loading;
    default_loading.append(NOT_USER_RECENT);

    array = setting.value(KEY_USER_CLOUD_STRUCT, default_loading).toByteArray();

    if(array == (WString)NOT_USER_RECENT){
        setting.endGroup();
        return NULL;
    }

    /*
     * either the data has been corrupted or the user
     * struct had to be changed and you have to
     * return null and delete the key inside the settings
    */
    if(array.size() != sizeof(struct struct_user)){
        remove_key(KEY_USER_CLOUD_STRUCT, GROUPNAME_CLOUD_USER);
        setting.setValue(KEY_USER_CLOUD_IS_DEFINED, false);
        setting.endGroup();

        return NULL;
    }

    setting.endGroup();

    /* memory of array.data() is automaticaly relised after function end */
    void *temp = array.data();

    user = new struct struct_user;

    memcpy(user, temp, sizeof(struct struct_user));

    return user;


}
#endif //CLOUD
