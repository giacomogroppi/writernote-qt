#ifndef PLATFORM_H
#define PLATFORM_H

#include "utils/common_script.h"
#include <QGuiApplication>

#if defined(WIN64) || defined(WIN32)
# define is_windows 1
#else
# define is_windows 0
#endif

#if defined(linux)
# define is_linux 1
#else
# define is_linux 0
#endif

#if defined(ANDROID_WRITERNOTE)
# define is_android 1
#else
# define is_android 0
#endif

#if defined(IOS_WRITERNOTE)
# define is_ios 1
#else
# define is_ios 0
#endif

force_inline bool is_mobile_static()
{
    return is_android || is_ios;
}

force_inline bool is_wayland()
{
    if(is_linux){
        return QGuiApplication::platformName().indexOf("wayland") != -1;
    }else{
        return 0;
    }
}

#endif // PLATFORM_H
