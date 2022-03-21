#include "stroke_drawer.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/page.h"
#include "touch/tabletcanvas.h"
#include "utils/common_script.h"
#include "qmath.h"

constexpr double deltaColorNull = 1.4;
constexpr double deltaPress = 2.;

force_inline void set_press(QPen &pen, const pressure_t press, const double prop, cbool is_rubber)
{
    pen.setWidth(TabletCanvas::pressureToWidth(press / deltaPress) * prop);
    if(unlikely(is_rubber)){
        const auto _press = pen.widthF() * deltaColorNull;
        pen.setWidthF(_press);
    }
}

force_inline void stroke_drawer::draw_circle(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop)
{
    constexpr bool debCircle = false;
    point_s point;

    stroke_complex_circle *data = (stroke_complex_circle *)stroke._complex;
    point._y = data->_y;
    point = page::at_translation(point, page);
    const auto y = point.y();

    const auto press = data->_press;

    set_press(pen, press, prop, is_rubber);
    painter.setPen(pen);

    painter.drawEllipse(QPointF(data->_x, y) * prop, data->_r * prop, data->_r * prop);

    WDebug(debCircle, __FUNCTION__ << data->_x << data->_y << data->_r);
}

force_inline void stroke_drawer::draw_stroke_normal(
        QPainter        &_painter,   const stroke    &stroke,
        cint            page,       QPen            &pen,
        cbool           is_rubber,  cdouble         prop)
{
    W_ASSERT(page >= 0);
    W_ASSERT(_painter.isActive());

    QImage img = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);
    Define_PAINTER_p(painter, img);
    const auto alfa = pen.color().alpha();
    int counterPoint;
    QPointF lastPoint, pointDraw;
    cint lenPoint = stroke.length();
    const QColor col = pen.color();
    cbool isHigh = col.alpha() < 255;

    lastPoint = page::at_translation(stroke.at(0), page).toQPointF(prop == PROP_RESOLUTION ? prop : 1.);

    for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
        const point_s point = page::at_translation(stroke.at(counterPoint), page);
        const pressure_t pressure = stroke.getPressure(counterPoint);

        pointDraw = point.toQPointF(prop == PROP_RESOLUTION ? prop : 1.);

        set_press(pen, pressure, prop, is_rubber);
        painter.setPen(pen);

        if(unlikely(is_rubber)){
            pen.setWidthF(pen.widthF() * deltaColorNull);
        }
        else if(unlikely(isHigh)){
            const QPainter::CompositionMode curr = painter.compositionMode();
            painter.setCompositionMode(QPainter::CompositionMode_Clear);
            painter.drawPoint(lastPoint);
            painter.setCompositionMode(curr);
        }

        painter.drawLine(lastPoint, pointDraw);

        lastPoint = pointDraw;
    }

    painter.end();

    _painter.drawImage(img.rect(), img);
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

    set_press(pen, data->press, prop, is_rubber);

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
    const auto press = data->press;

    set_press(pen, press, prop, is_rubber);

    painter.setPen(pen);
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
