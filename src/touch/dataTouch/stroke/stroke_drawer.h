#pragma once

#include "core/WList.h"
#include "touch/dataTouch/Point.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "core/WPainter/WPainter.h"
#include "core/Pen/WPen.h"

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
    WPainter    &_painter;
    WPen        &_pen;
    int         _page;
    double      _prop;
    WColor      _color;
    bool        _isRubber;

    template <class T, class Z>
    void draw_stroke_normal(stroke_drawer_private<T, Z> &data);

    stroke_drawer(WPainter &painter, WPen &pen, double prop, const WColor &color, int page, bool is_rubber);
public:
    static void draw_stroke(WPainter &painter, const StrokePre &stroke, WPen &pen, cdouble prop);
};

inline stroke_drawer::stroke_drawer(WPainter &painter, WPen &pen, double prop, const WColor &color, int page, bool is_rubber) :
    _painter(painter),
    _pen(pen),
    _page(page),
    _prop(prop),
    _color(color),
    _isRubber(is_rubber)
{
}

