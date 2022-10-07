#pragma once

#include <QWidget>
#include "utils/common_script.h"
#include "utils/platform.h"
#include "utils/utils.h"
#include <QPainter>

extern bool __is_mobile_view;
extern class MainWindow *__private_mainwindow;
extern class TabletCanvas *__private_canvas;

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
    W_ASSERT(__private_mainwindow == nullptr);
    __private_mainwindow = main;
}

force_inline class MainWindow *get_main_window()
{
    W_ASSERT(__private_mainwindow != nullptr);
    return __private_mainwindow;
}

force_inline void set_canvas(class TabletCanvas *canvas)
{
    W_ASSERT(__private_canvas == nullptr);
    __private_canvas = canvas;
}

force_inline class TabletCanvas *get_canvas()
{
    W_ASSERT(__private_canvas != nullptr);
    return __private_canvas;
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
QPoint get_pos_start_mouse();

#define TRANSLATION(x) QApplication::tr(x)

inline void painter_set_source_over(QPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}

inline void painter_set_antialiasing(QPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
}

}

