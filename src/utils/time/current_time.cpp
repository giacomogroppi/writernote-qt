#include "current_time.h"

QTime current_time()
{
    QTime temp;
    temp.currentTime();
    return temp;

}

QString current_time_string(){
    QTime temp = current_time();
    return temp.toString();
}
