#include "stroke_drawer.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/page.h"
#include "touch/tabletcanvas.h"

force_inline void stroke_drawer::draw_circle(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop)
{
    painter.setPen(pen);
    stroke_complex_circle *data = (stroke_complex_circle *)stroke._complex;
    painter.drawEllipse(QPointF(data->_x, data->_y) * prop, data->_r * prop, data->_r * prop);
}

force_inline void stroke_drawer::draw_const(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cdouble prop)
{
    const QPainterPath &path = stroke.getQPainterPath(page);

    painter.strokePath(path, pen);
}

force_inline void stroke_drawer::draw_not_const(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop)
{
    int counterPoint;
    constexpr double deltaColorNull = 1.4;
    QPointF lastPoint, pointDraw;

    cint lenPoint = stroke.length();

    lastPoint = page::at_translation(stroke.at(0), page).toQPointF(prop);

    for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
        const point_s point = page::at_translation(stroke.at(counterPoint), page);
        const pressure_t pressure = stroke.getPressure(counterPoint);

        pointDraw = point.toQPointF(prop);

        pen.setWidthF(TabletCanvas::pressureToWidth(pressure / 2.00) * prop);

        if(unlikely(is_rubber)){
            pen.setWidthF(pen.widthF() * deltaColorNull);
        }

        painter.setPen(pen);

        painter.drawLine(lastPoint, pointDraw);

        lastPoint = pointDraw;
    }
}

force_inline void stroke_drawer::draw_stroke_normal(
        QPainter        &painter,
        const stroke    &stroke,
        cint            page,
        QPen            &pen,
        cbool           is_rubber,
        cdouble         prop)
{
    W_ASSERT(page >= 0);
    W_ASSERT(painter.isActive());

    if(prop == PROP_RESOLUTION && stroke.constantPressure()){
        stroke_drawer::draw_const(painter, stroke, page, pen, prop);
    }else{
        stroke_drawer::draw_not_const(painter, stroke, page, pen, is_rubber, prop);
    }
}

force_inline void stroke_drawer::draw_rect(
        QPainter        &painter,
        const stroke    &stroke,
        cint            page,
        QPen            &pen,
        cbool           is_rubber,
        cdouble         prop)
{
    pen.setColor(stroke.getColor());
    painter.setPen(pen);
    stroke_complex_rect *data = (stroke_complex_rect *)stroke._complex;

    painter.drawRect(QRectF(data->rect.topLeft() * prop, data->rect.bottomRight() * prop));
}

force_inline void stroke_drawer::draw_line(
        QPainter        &painter,
        const stroke    &stroke,
        cint            page,
        QPen            &pen,
        cbool           is_rubber,
        cdouble         prop)
{
    pen.setColor(stroke.getColor());
    painter.setPen(pen);
    stroke_complex_line * data = (stroke_complex_line *)stroke._complex;

    painter.drawLine(data->topLeft * prop, data->bottomRight * prop);
}


void stroke_drawer::draw_stroke(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop)
{
    if(likely(stroke.is_normal())){
        stroke_drawer::draw_stroke_normal(painter, stroke, page, pen, is_rubber, prop);
    }else if(stroke.is_circle()){
        stroke_drawer::draw_circle(painter, stroke, page, pen, is_rubber, prop);
    }else if(stroke.is_rect()){
        stroke_drawer::draw_rect(painter, stroke, page, pen, is_rubber, prop);
    }else if(stroke.is_line()){
        stroke_drawer::draw_line(painter, stroke, page, pen, is_rubber, prop);
    }else{
        std::abort();
    }
}
