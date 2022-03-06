#include "stroke_drawer.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/page.h"
#include "touch/tabletcanvas.h"

force_inline void stroke_drawer::draw_circle(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber)
{
    Q_UNUSED(painter);
    Q_UNUSED(stroke);
    Q_UNUSED(page);
    Q_UNUSED(pen);
    Q_UNUSED(is_rubber);
}

force_inline void stroke_drawer::draw_const(QPainter &painter, const stroke &stroke, cint page, QPen &pen)
{
    const QPainterPath &path = stroke.getQPainterPath(page);

    painter.strokePath(path, pen);
}

force_inline void stroke_drawer::draw_not_const(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber)
{
    int counterPoint;
    constexpr double deltaColorNull = 1.4;
    QPointF lastPoint, pointDraw;

    cint lenPoint = stroke.length();

    lastPoint = page::at_translation(stroke.at(0), page).toQPointF(PROP_RESOLUTION);

    for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
        const point_s point = page::at_translation(stroke.at(counterPoint), page);
        const pressure_t pressure = stroke.getPressure(counterPoint);

        pointDraw = point.toQPointF(PROP_RESOLUTION);

        pen.setWidthF(TabletCanvas::pressureToWidth(pressure / 2.00) * PROP_RESOLUTION);

        if(unlikely(is_rubber)){
            pen.setWidthF(pen.widthF() * deltaColorNull);
        }

        painter.setPen(pen);

        painter.drawLine(lastPoint, pointDraw);

        lastPoint = pointDraw;
    }
}

force_inline void stroke_drawer::draw_stroke_normal(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber)
{

    const bool constantPressure = stroke.constantPressure();

    W_ASSERT(page >= 0);
    W_ASSERT(painter.isActive());

    if(constantPressure){
        stroke_drawer::draw_const(painter, stroke, page, pen);
    }else{
        stroke_drawer::draw_not_const(painter, stroke, page, pen, is_rubber);
    }
}

force_inline void stroke_drawer::draw_rect(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber)
{
    Q_UNUSED(painter);
    Q_UNUSED(stroke);
    Q_UNUSED(page);
    Q_UNUSED(pen);
    Q_UNUSED(is_rubber);
}

void stroke_drawer::draw_stroke(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber)
{
    if(likely(stroke.is_normal())){
        stroke_drawer::draw_stroke_normal(painter, stroke, page, pen, is_rubber);
    }else if(stroke.is_circle()){
        stroke_drawer::draw_circle(painter, stroke, page, pen, is_rubber);
    }else if(stroke.is_rect()){
        stroke_drawer::draw_rect(painter, stroke, page, pen, is_rubber);
    }
}
