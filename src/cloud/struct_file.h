#pragma once

#include "struct_user.h"

#ifdef CLOUD

#define OWNER_YOU 0
#define OWNER_FRIEND 1

#define PERMISSION_ALL 2
#define PERMISSION_READONLY 3
#define PERMISSION_WRITING 4

/*
 * share only the version of the file
 * or share also all the modification
*/
#define SHARE_ALL_VERION 1
#define SHARE_ONLY_ONE_VERSION 2

struct struct_file{
    struct struct_user;
    int type_owner;

    /* if type_owner == WONER_YOU the permission is ALL */
    int type_permission;

    /* if  type_owner == OWNER_YOU -> type_share == SHArE_ALL_VERSION */
    int type_share;
};


struct struct_file * bytearray_tostruct_file(QByteArray &);

#endif // CLOUD
