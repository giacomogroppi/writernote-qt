#pragma once

#include <QObject>
#include "utils/WCommonScript.h"
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

#define TRANSLATION(x) QApplication::tr(x)

inline void painter_set_source_over(QPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    W_ASSERT(painter.compositionMode() == QPainter::CompositionMode_SourceOver);
}

inline void painter_set_antialiasing(QPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);
}

inline double scaleFactor()
{
#ifdef MACOS
    return 2.;
#else
    return 1.;
#endif
}

}

