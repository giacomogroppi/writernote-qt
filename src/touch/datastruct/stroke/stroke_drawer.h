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

class stroke_drawer {
private:
    template <class T>
    class stroke_drawer_private_iteration{
    private:
        typename WList<T>::ConstIterator _begin;
        typename WList<T>::ConstIterator _end;
    public:
        stroke_drawer_private_iteration(typename WList<T>::ConstIterator begin, typename WList<T>::ConstIterator end) {
            this->_begin = begin;
            this->_end = end;
        };
    };

    class stroke_drawer_private{
    private:
        WList<point_s>::ConstIterator       _pointBegin;
        WList<point_s>::ConstIterator       _pointEnd;
        WList<pressure_t>::ConstIterator    _pressureBegin;
        WList<pressure_t>::ConstIterator    _pressureEnd;
        bool                                _pressureNull;
    public:
        stroke_drawer_private(WList<point_s>::ConstIterator pointBegin, WList<point_s>::ConstIterator pointEnd,
                              WList<pressure_t>::ConstIterator pressureBegin, WList<pressure_t>::ConstIterator pressureEnd,
                              bool pressureNull):
                _pointBegin(pointBegin),
                _pointEnd(pointEnd),
                _pressureBegin(pressureBegin),
                _pressureEnd(pressureEnd),
                _pressureNull(pressureNull)
        {
        };
    };

    //static void draw_stroke_normal();
    static void draw_stroke_normal(QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);

    // complex stroke
    static void draw_circle (QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
    static void draw_rect   (QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
    static void draw_line   (QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
public:
    static void draw_stroke(QPainter &painter, const StrokePre &stroke, QPen &pen, cdouble prop);
    static void draw_stroke(QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
};

