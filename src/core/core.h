#ifndef CORE_H
#define CORE_H

#include "qwidget.h"
#include "utils/common_script.h"
#include "utils/platform.h"
#include "utils/utils.h"

extern bool __is_mobile_view;
extern class MainWindow *__private_mainwindow;

namespace core{

force_inline bool is_mobile_view()
{
#if is_android || is_ios
    return true;
#else
    return __is_mobile_view;
#endif
}

force_inline void set_mobile_view(cbool is_tablet)
{
    if(!is_mobile_static())
        __is_mobile_view = is_tablet;
}

force_inline void set_main_window(class MainWindow *main)
{
    W_ASSERT(__private_mainwindow == NULL);
    __private_mainwindow = main;
}

force_inline class MainWindow *get_main_window()
{
    W_ASSERT(__private_mainwindow != NULL);
    return __private_mainwindow;
}

force_inline void set_max_size_as_screen(QWidget *widget)
{
    W_ASSERT(widget);
    if(is_wayland()){
        const auto maxSize = utils::get_size_screen();
        widget->setMaximumSize(maxSize);
    }
}

bool is_dark_mode();
void adjustDarkMode();

}


#endif // CORE_H
