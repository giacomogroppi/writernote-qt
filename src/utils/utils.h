#pragma once


#include <QSize>
#include "qapplication.h"
#include "utils/common_script.h"
#include <QList>
#include <QScreen>
#include <chrono>

namespace utils{

force_inline QSize get_size_screen()
{
    QSize size = qApp->screens()[0]->size();
    return size;
}

force_inline unsigned long int get_time_since_epoch(){
    using namespace std::chrono;
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}

}

