#include "stroke_drawer.h"
#include "touch/dataTouch/page/Page.h"
#include "touch/tabletcanvas.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/dataTouch/stroke/stroke_complex_data.h"

template <class T, class Z>
void stroke_drawer::draw_stroke_normal(stroke_drawer_private<T, Z> &data)
{
    constexpr bool not_used debug_draw_stroke = true;
    auto &_painterPublic = this->_painter;

    W_ASSERT(this->_page >= 0);
    W_ASSERT(_painterPublic.isActive());

    WImage img;
    QPainter _painterPrivate;
    QPainter *painter;
    QPointF lastPoint, pointDraw;
    cbool isHigh = _pen.color().alpha() < 255;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;
    cbool isPrivatePainter = isHigh;

    if(isPrivatePainter){
        img = WImage(1);
        _painterPrivate.begin(&img);
        SetRenderPainter(_painterPrivate);
        painter = &_painterPrivate;
    }else{
        painter = &_painterPublic;
    }

    lastPoint = Page::at_translation(*data._pointBegin, this->_page).toQPointF(prop);

    for(data._pointBegin ++; data._pointBegin != data._pointEnd; data._pointBegin ++){
        const point_s point = Page::at_translation(*data._pointBegin, this->_page);
        const pressure_t pressure = *data._pressureBegin;

        if(!data._pressureNull){
            data._pressureBegin ++;
        }

        pointDraw = point.toQPointF(prop);

        set_press(this->_pen, pressure, _prop, this->_isRubber, this->_color);
        painter->setPen(this->_pen);

        if(unlikely(this->_isRubber)){
            this->_pen.setWidthF(_pen.widthF() * deltaColorNull);
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

    if(likely(isPrivatePainter)){
        W_ASSERT(isHigh);
        W_ASSERT(_painterPublic.compositionMode() == QPainter::CompositionMode_SourceOver);

        WDebug(debug_draw_stroke, "Paint high" << _painterPublic.compositionMode());

        painter->end();

        _painterPublic.drawImage(img.rect(), img);
    } else {
        WDebug(debug_draw_stroke, "Paint not high");
    }
}

force_inline void stroke_drawer::draw_rect(const Stroke &stroke)
{
    _painter.setPen(_pen);
    auto *data = static_cast<stroke_complex_rect *>(stroke._complex);

    set_press(_pen, data->press, _prop, _isRubber, this->_color);

    const auto _topLeft     = Page::at_translation(
                point_s(
                    data->rect.topLeft()),
                    _page
                ).toQPointF(_prop);

    const auto _bottomRight = Page::at_translation(
                point_s(
                    data->rect.bottomRight()),
                    _page
                ).toQPointF(_prop);

    _painter.drawRect(QRectF(_topLeft, _bottomRight));
}

force_inline void stroke_drawer::draw_line(const Stroke &stroke)
{
    auto * data = static_cast<stroke_complex_line *>(stroke._complex);
    const auto press = data->press;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;

    _painter.setPen(_pen);
    set_press(_pen, press, _prop, _isRubber, this->_color);

    const auto _topLeft     = Page::at_translation(point_s(data->pt1), _page).toQPointF(prop);
    const auto _bottomRight = Page::at_translation(point_s(data->pt2), _page).toQPointF(prop);

    _painter.setPen(_pen);
    _painter.drawLine(_topLeft, _bottomRight);
}

void stroke_drawer::draw_stroke(QPainter &painter, const StrokePre &stroke, QPen &pen, cdouble prop)
{
    stroke_drawer d(painter, pen, prop, stroke.getColor(), 0, false);

    W_ASSERT(painter.isActive());

    WDebug(false, "Print" << dynamic_cast<const StrokeProp &>(stroke).toString());

    if(likely(stroke.is_normal())){
        stroke_drawer_private<WList<point_s>, WList<pressure_t>> data(
                stroke.get_last_point(),
                stroke._point.constEnd(),
                stroke.get_last_press(),
                stroke._pressure.constEnd(),
                false
        );

        if(stroke._point.length() == 1)
            return;
        d.draw_stroke_normal(data);
    }else{
        const auto &s = stroke.get_stroke_for_draw();
        if(stroke.is_circle()){
            d.draw_circle(s);
        }else if(stroke.is_rect()){
            d.draw_rect(s);
        }else if(stroke.is_line()){
            d.draw_line(s);
        }else{
            std::abort();
        }
    }
}

void stroke_drawer::draw_stroke(QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop)
{
    stroke_drawer d(painter, pen, prop, stroke.getColor(), page, is_rubber);

    if(likely(stroke.is_normal())){
        stroke_drawer_private<
                QList<point_s>, QList<pressure_t>
        > data(
                stroke._point.constBegin(),
                stroke._point.constEnd(),
                stroke._pressure.constBegin(),
                stroke._pressure.constEnd(),
                stroke.constantPressure()
        );

        d.draw_stroke_normal(data);
    }else if(stroke.is_circle()){
        d.draw_circle(stroke);
    }else if(stroke.is_rect()){
        d.draw_rect(stroke);
    }else if(stroke.is_line()){
        d.draw_line(stroke);
    }else{
        std::abort();
    }
}
