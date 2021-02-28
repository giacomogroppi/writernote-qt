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

#define SPACER ":"
#define TOSTRING(x) QString::number(x)

#define C 3

static QString to_string(int);

QString current_day_string(){
    int day[C];
    day[0] = QDate::currentDate().year();
    day[1] = QDate::currentDate().month();
    day[2] = QDate::currentDate().day();

    QString string_return = "";
    for(int i=0; i<C; i++){
        string_return += to_string(day[i]) + SPACER;
    }

    return string_return;
}

static QString to_string(int num){
    QString t = TOSTRING(num);
    if(t.length() == 1)
        t = "0" + t;

    return t;
}
