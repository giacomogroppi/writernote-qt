#include "touch/object_finder/model/model.h"
#include "testing/memtest.h"
#include <qmath.h>
#include "utils/WCommonScript.h"
#include "core/WLine.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/dataTouch/stroke/stroke_complex_data.h"

constexpr double    error = 5000;
constexpr bool      debug = false;

struct{
    double m, q;
    bool is_vertical;
} line_data;

static void is_near_line(cdouble m, double &max, cdouble q, const point_s *point)
{
    const auto x = point->x();
    const auto y = point->y();
    double res;

    if(line_data.is_vertical){
        res = qAbs(q - x);
        if(res > max){
            max = res;
        }
    }else{
        res = qAbs(x * m + q - y);
        if(res > max){
            max = res;
        }
    }
}

static not_used bool line_check_segno(cdouble segno, cdouble len)
{
    /**
     * se più del 5% dei punti presenta un "ritorno"
     * e è minore del 95 % allora non è una linea
    */
    const double mod = qAbs(segno);
    return mod > len * .05 && 
           mod < len * .95;
}

static force_inline void model_line_adjust_m(double &m)
{
    const double mod = qAbs(m);
    if(mod < 0.07){
        m = 0;
    }
    if(mod > 25.){
        line_data.is_vertical = true;
    }
}

double model_line(const StrokePre *stroke)
{

    int segno_var_x, segno_var_y;
    int i;
    const point_s *one, *two;
    const auto &area = stroke->getFirstAndLast();
    auto b = stroke->constBegin();
    const auto e = stroke->constEnd();

    double &m = line_data.m;
    double &q = line_data.q;
    bool &is_vertical = line_data.is_vertical;

    double precision = 0;

    line_data.is_vertical = false;

    /** list empty */
    if(unlikely(b == e)){
        return error;
    }

    //WDebug(debug, __FUNCTION__ << area.topLeft() << area.bottomRight());

    {
        const double det = area.topLeft().x() - area.bottomRight().x();

        if(unlikely(!det)){
            WDebug(debug, "det = 0");
            is_vertical = true;
            goto cont;
        }

        m = (area.topLeft().y() - area.bottomRight().y()) / det;
    }

    model_line_adjust_m(m);

    WDebug(debug, qstr("m: %1 q: %2 is_vertical %3").arg(m).arg(q).arg(line_data.is_vertical));

cont:
    segno_var_x = segno_var_y = 0;

    one = & (*b);
    for(b ++; b != e; b ++){
        two = & (*b);

        // calcolo variazione di segno
        if(two->x() - one->x() < 0){
            segno_var_x ++;
        }
        if(two->y() - one->y() < 0){
            segno_var_y ++;
        }

        one = two;
    }

    WDebug(debug, "segno_var" << segno_var_x << segno_var_y);

    /*if( line_check_segno(segno_var_x, len) ||
        line_check_segno(segno_var_y, len))
        return error;*/

    if(segno_var_y < 0){
        /*
         * se segno_var_y è minore di 0 allora la retta che l'utente
         * ha disegnato va dall'alto a sinistra in basso a destra
        */
        m = 1. / m;
    }

    one = & (*stroke->constBegin());

    if(is_vertical){
        q = one->x();
    }
    else{
        q = one->y() - (one->x()) * m;
    }


    for( b = stroke->constBegin(); b != e; b++ ){
        one = & (*b);

        is_near_line(m, precision, q, one);
    }

    WDebug(debug, qstr("Line precision: %1").arg(precision));

    return precision;
}

static void model_line_vertical(StrokePre *stroke, stroke_complex_line *data)
{
    const auto press = stroke->getPressure();
    const QRect FL = stroke->getFirstAndLast();
    QPointF TL = FL.topLeft();
    QPointF BR = FL.bottomRight();

    if(TL.y() > BR.y()){
        WCommonScript::swap(TL, BR);
    }

    const double x = TL.x();

    stroke->reset();

    data->pt1 = QPointF(x, TL.y());
    data->pt2 = QPointF(x, BR.y());
    data->press = press;
}

static void model_line_generic(StrokePre *stroke, stroke_complex_line *data)
{
    const auto pressure = stroke->getPressure();
    data->pt1           = stroke->constBegin()->toQPointF(1.);
    data->pt2           = stroke->last().       toQPointF(1.);

    if(data->pt1.y() > data->pt2.y()){
        WCommonScript::swap(data->pt1, data->pt2);
    }

    data->press = pressure;
    stroke->reset();
}

void model_line_create(StrokePre *stroke)
{
    W_ASSERT(stroke);
    bool &is_vertical = line_data.is_vertical;
    auto *data = (stroke_complex_line *)
            WMalloc(
                        sizeof(stroke_complex_line)
                    );

    if(is_vertical){
        model_line_vertical(stroke, data);
    }else{
        model_line_generic(stroke, data);
    }

    stroke->reset();
    stroke->set_complex(StrokeProp::COMPLEX_LINE, data);
}

void stroke_complex_line_append(Stroke *stroke, const QPointF& point)
{
    auto *data = (stroke_complex_line *) stroke->get_complex_data();
    const auto dist1 = WCommonScript::distance_not_square(data->pt1, point);
    const auto dist2 = WCommonScript::distance_not_square(data->pt2, point);

    if(dist1 > dist2){
        data->pt2 = point;
    }else{
        data->pt1 = point;
    }
}

bool stroke_complex_is_inside_line(const Stroke *stroke, const WLine &line, cdouble precision)
{
    const auto data = (const stroke_complex_line *)stroke->get_complex_data();

    W_ASSERT(stroke->is_line());

    WLine _line(data->pt1, data->pt2);
    return WLine::intersect(_line, line, precision);
}

void stroke_complex_translate_line(Stroke *stroke, const QPointF &offset)
{
    stroke_complex_line *data = (stroke_complex_line *)stroke->get_complex_data();
    data->pt1 += offset;
    data->pt2 += offset;
}

static inline void stroke_complex_normal_line_generic(
        Stroke              *_to,
        stroke_complex_line *data,
        int from, int to)
{
    double m, p;
    point_s point;
    const pressure_t press = data->press;

    W_ASSERT(from <= to);

    m =     (data->pt1.y() - data->pt2.y()) /
            (data->pt1.x() - data->pt2.x());

    p = data->pt1.y() - data->pt1.x() * m;

    for(; from <= to; from ++){
        const double x = (double(from) - p) / m;
        point = point_s(
                x,
                static_cast<double>(from)
                );
        _to->append(point, press);
    }
}

static inline void stroke_complex_normal_line_vertical(
        Stroke              *_to,
        stroke_complex_line *data,
        int from, int to)
{
    point_s tmp;
    const pressure_t press = data->press;
    W_ASSERT(data->pt2.x() == data->pt1.x());
    W_ASSERT(from <= to);

    tmp.rx() = data->pt1.x();

    for(; from <= to; from ++){
        tmp.ry() = from;
        _to->append(tmp, press);
    }
}

void stroke_complex_make_normal_line   (const Stroke *_from, Stroke *_to)
{
    int from, to;
    stroke_complex_line *data = (stroke_complex_line *)_from->get_complex_data();
    W_ASSERT(_from->is_line());
    _to->reset();

    from    = (int) data->pt1.y();
    to      = (int) data->pt2.y();

    W_ASSERT(from <= to);

    if(data->pt2.x() == data->pt1.x()){
        stroke_complex_normal_line_vertical(_to, data, from, to);
    }else{
        stroke_complex_normal_line_generic(_to, data, from, to);
    }

    W_ASSERT(_to->is_normal());
}

bool stroke_complex_is_inside_line   (const Stroke *_stroke, const QRectF &area, cdouble precision)
{
    W_ASSERT(_stroke->is_line());

    const auto *data = (const stroke_complex_line *)_stroke->get_complex_data();
    //const auto rect = datastruct_rect(data->pt1, data->pt2);

    //WLine lineRight (rect.topRight(), rect.bottomRight());
    //WLine lineLeft  (rect.topLeft(), rect.bottomLeft());
    WLine lineOrizTop   (area.topLeft(),    area.topRight());
    WLine lineOrizBot   (area.bottomLeft(), area.bottomRight());
    WLine lineVertLeft  (area.topLeft(),    area.bottomLeft());
    WLine lineVertRig   (area.topRight(),   area.bottomRight());
    WLine _this         (data->pt1, data->pt2);

    // if the square passed to the function contains one of the two points
    if(area.contains(data->pt1)){
        WDebug(debug, "Contains first line" << data->pt1);
        return true;
    }
    if(area.contains(data->pt2)){
        WDebug(debug, "contains second line" << data->pt2);
        return true;
    }

    WDebug(debug, "intersect topLeft" << data->pt1 << "bottomr" << data->pt2 << "(rect)" << area);

    if(WLine::intersect(_this, lineOrizTop, precision)){
        WDebug(debug, "intersect 1");
        return true;
    }

    if(WLine::intersect(_this, lineOrizBot, precision)){
        WDebug(debug, "intersect 2")
        return true;
    }

    if(WLine::intersect(_this, lineVertLeft, precision)){
        WDebug(debug, "intersect 3");
        return true;
    }

    if(WLine::intersect(_this, lineVertRig, precision)){
        WDebug(debug, "intersect 4");
        return true;
    }

    return false;
}
