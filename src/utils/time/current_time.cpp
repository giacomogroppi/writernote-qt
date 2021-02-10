#include "current_time.h"

QTime current_time()
{
    return QTime::currentTime();
}

QString current_time_string(){
    return QTime::currentTime().toString();
}
