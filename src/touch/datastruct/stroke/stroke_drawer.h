#pragma once

#include "core/WList.h"
#include "touch/datastruct/point.h"
#include "utils/common_script.h"
#include "touch/datastruct/utils_datastruct.h"
#include <QtGlobal>
#include <QPainter>
#include <QPen>

class Stroke;
class StrokePre;

template <class T, class Z>
class stroke_drawer_private{
private:
    typename T::const_iterator   _pointBegin;
    typename T::const_iterator   _pointEnd;
    typename Z::const_iterator   _pressureBegin;
    typename Z::const_iterator   _pressureEnd;
    bool                        _pressureNull;
public:
    stroke_drawer_private(typename T::const_iterator pointBegin,     typename T::const_iterator pointEnd,
                          typename Z::const_iterator pressureBegin,  typename Z::const_iterator pressureEnd,
                          bool pressureNull):
            _pointBegin(pointBegin),
            _pointEnd(pointEnd),
            _pressureBegin(pressureBegin),
            _pressureEnd(pressureEnd),
            _pressureNull(pressureNull)
    {
    };

    friend class stroke_drawer;
};

class stroke_drawer {
private:
    QPainter    &_painter;
    QPen        &_pen;
    int         _page;
    double      _prop;
    QColor      _color;
    bool        _isRubber;

    template <class T, class Z>
    void draw_stroke_normal(stroke_drawer_private<T, Z> &data);

    // complex stroke
    void draw_circle (const Stroke &stroke);
    void draw_rect   (const Stroke &stroke);
    void draw_line   (const Stroke &stroke);

    stroke_drawer(QPainter &painter, QPen &pen, double prop, const QColor &color, int page, bool is_rubber);
public:
    static void draw_stroke(QPainter &painter, const StrokePre &stroke, QPen &pen, cdouble prop);
    static void draw_stroke(QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
};

inline stroke_drawer::stroke_drawer(QPainter &painter, QPen &pen, double prop, const QColor &color, int page, bool is_rubber) :
    _painter(painter),
    _pen(pen),
    _prop(prop),
    _color(color),
    _page(page),
    _isRubber(is_rubber)
{
}

