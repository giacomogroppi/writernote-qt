#include "current_time.h"

QTime current_time()
{
    return QTime::currentTime();
}

QString current_time_string(){
    return QTime::currentTime().toString();
}

QDate current_day(){
    return QDate::currentDate();
}

QString current_day_string(){
    return current_day().toString();
}
