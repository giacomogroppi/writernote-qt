#pragma once

#include "utils/WCommonScript.h"
#include "utils/platform.h"
#include "utils/utils.h"
#include "core/WPainter/WPainter.h"
#include "Scheduler/WObject.h"

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

#if defined(USE_QT)
inline void painter_set_source_over(WPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setCompositionMode(WPainter::CompositionMode_SourceOver);
    W_ASSERT(painter.compositionMode() == WPainter::CompositionMode_SourceOver);
}

inline void painter_set_destination_over(WPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setCompositionMode(WPainter::CompositionMode_DestinationOver);
}

inline void painter_set_antialiasing(WPainter &painter)
{
    W_ASSERT(painter.isActive());
    painter.setRenderHints(WPainter::RenderHint::Antialiasing);
    painter.setRenderHint(WPainter::RenderHint::SmoothPixmapTransform);
    painter.setRenderHint(WPainter::RenderHint::TextAntialiasing);

    W_ASSERT(painter.renderHints() & WPainter::RenderHint::SmoothPixmapTransform);
    W_ASSERT(painter.renderHints() & WPainter::RenderHint::Antialiasing);
    W_ASSERT(painter.renderHints() & WPainter::RenderHint::TextAntialiasing);
}
#endif // USE_QT

inline double scaleFactor()
{
#ifdef MACOS
    return 2.;
#else
    return 1.;
#endif
}

}

