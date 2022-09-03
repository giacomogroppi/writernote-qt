#pragma once

#include <QString>
#include "cloud/struct_user.h"
#include "shared_definition.h"

#ifdef CLOUD

enum downloadfile{
    not_register,
    passoword_wronge,
    server_error,
    file_not_exist,
    ok
};

downloadfile dowloadfile(struct struct_user *, const char *namefile, QString &to);

#endif // CLOUD
