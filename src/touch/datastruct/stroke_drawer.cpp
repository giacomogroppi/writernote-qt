#include "stroke_drawer.h"
#include "touch/datastruct/stroke.h"
#include "touch/datastruct/page.h"
#include "touch/tabletcanvas.h"
#include "utils/common_script.h"
#include "qmath.h"

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
constexpr double deltaColorNull = 1.08;
#else
constexpr double deltaColorNull = 1.3;
#endif
constexpr double deltaPress = 2.;

force_inline void set_press(QPen &pen, const pressure_t press, const double prop, cbool is_rubber)
{
    pen.setWidth(TabletCanvas::pressureToWidth(press / deltaPress) * prop);
    if(unlikely(is_rubber)){
        const auto _press = pen.widthF() * deltaColorNull;
        pen.setWidthF(_press);
    }
}

force_inline void stroke_drawer::draw_circle(
        QPainter    &painter,  const stroke    &stroke,
        cint        page,      QPen            &pen,
        cbool       is_rubber, cdouble         _prop)
{
    constexpr bool not_used debCircle = false;
    point_s point;
    QPointF tmp;
    auto *data = (stroke_complex_circle *)stroke._complex;
    const auto press = data->_press;
    double y, x;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;

    point._x = data->_x;
    point._y = data->_y;

    point = page::at_translation(point, page);
    tmp = point.toQPointF(prop);

    WDebug(debCircle, "prop: " << prop);

    y = tmp.y();
    x = tmp.x();

    set_press(pen, press, prop, is_rubber);
    painter.setPen(pen);

    painter.drawEllipse(QPointF(x, y), data->_r * prop, data->_r * prop);

    WDebug(debCircle, data->_x << data->_y << data->_r);
}

force_inline void stroke_drawer::draw_stroke_normal(
        QPainter        &_painterPublic,   const stroke    &stroke,
        cint            page,       QPen            &pen,
        cbool           is_rubber,  cdouble         _prop)
{
    constexpr bool not_used debug_draw_stroke = false;

    W_ASSERT(page >= 0);
    W_ASSERT(_painterPublic.isActive());

    QImage img;
    QPainter _painterPrivate;
    QPainter *painter;
    int counterPoint;
    QPointF lastPoint, pointDraw;
    cint lenPoint = stroke.length();
    const QColor col = pen.color();
    cbool isHigh = col.alpha() < 255;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;
    cbool isPrivatePainter = isHigh;

    if(isPrivatePainter){
        img = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);
        _painterPrivate.begin(&img);
        SetRenderPainter(_painterPrivate);
        painter = &_painterPrivate;
    }else{
        painter = &_painterPublic;
    }

    lastPoint = page::at_translation(stroke.at(0), page).toQPointF(prop);

    for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
        const point_s point = page::at_translation(stroke.at(counterPoint), page);
        const pressure_t pressure = stroke.getPressure(counterPoint);

        pointDraw = point.toQPointF(prop);

        set_press(pen, pressure, _prop, is_rubber);
        painter->setPen(pen);

        if(unlikely(is_rubber)){
            pen.setWidthF(pen.widthF() * deltaColorNull);
        }
        else if(unlikely(isHigh)){
            const QPainter::CompositionMode curr = painter->compositionMode();
            painter->setCompositionMode(QPainter::CompositionMode_Clear);
            painter->drawPoint(lastPoint);
            painter->setCompositionMode(curr);
        }

        painter->drawLine(lastPoint, pointDraw);

        lastPoint = pointDraw;
    }

    if(unlikely(!isPrivatePainter)){
        return;
    }

    W_ASSERT(isHigh);
    W_ASSERT(_painterPublic.compositionMode() == QPainter::CompositionMode_SourceOver);

    WDebug(debug_draw_stroke, "Paint high");

    painter->end();

    _painterPublic.drawImage(img.rect(), img);
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

    const auto _topLeft     = page::at_translation(point_s(data->rect.topLeft()), page).toQPointF(prop);
    const auto _bottomRight = page::at_translation(point_s(data->rect.bottomRight()), page).toQPointF(prop);

    painter.drawRect(QRectF(_topLeft, _bottomRight));
}

force_inline void stroke_drawer::draw_line(
        QPainter        &painter,
        const stroke    &stroke,
        cint            page,
        QPen            &pen,
        cbool           is_rubber,
        cdouble         _prop)
{
    pen.setColor(stroke.getColor());
    painter.setPen(pen);
    stroke_complex_line * data = (stroke_complex_line *)stroke._complex;
    const auto press = data->press;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;

    set_press(pen, press, _prop, is_rubber);

    const auto _topLeft     = page::at_translation(point_s(data->topLeft), page).toQPointF(prop);
    const auto _bottomRight = page::at_translation(point_s(data->bottomRight), page).toQPointF(prop);

    painter.setPen(pen);
    painter.drawLine(_topLeft, _bottomRight);
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
