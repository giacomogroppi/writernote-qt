#include "get_path_application.h"
#include "assert.h"
#include <QStandardPaths>

QString get_path_application::exe(){
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

