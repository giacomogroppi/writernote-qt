#include "touch/object_finder/model/model.h"
#include "touch/datastruct/datastruct.h"
#include "testing/memtest.h"
#include <qmath.h>
#include "utils/common_script.h"
#include "core/wline.h"

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
    /*
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

double model_line(const stroke *stroke)
{
    int segno_var_x, segno_var_y;
    int i, len;
    const point_s *one, *two;
    const auto &area = stroke->getFirstAndLast();

    double &m = line_data.m;
    double &q = line_data.q;
    bool &is_vertical = line_data.is_vertical;

    double precision = 0;

    line_data.is_vertical = false;

    len = stroke->length();

    if(unlikely(!len)){
        return error;
    }

    //WDebug(debug, __FUNCTION__ << area.topLeft() << area.bottomRight());

    {
        const double det = area.topLeft().x() - area.bottomRight().x();

        if(unlikely(!det)){
            WDebug(debug, __FUNCTION__ << "det = 0");
            is_vertical = true;
            goto cont;
        }

        m = (area.topLeft().y() - area.bottomRight().y()) / det;
    }

    model_line_adjust_m(m);

    WDebug(debug, __FUNCTION__ << qstr("m: %1 q: %2 is_vertical %3").arg(m).arg(q).arg(line_data.is_vertical));

cont:
    segno_var_x = segno_var_y = 0;

    one = &stroke->at(0);
    for(i = 1; i < len; i++){
        two = & stroke->at(i);

        // calcolo variazione di segno
        if(two->x() - one->x() < 0){
            segno_var_x ++;
        }
        if(two->y() - one->y() < 0){
            segno_var_y ++;
        }

        one = two;
    }

    WDebug(debug, __FUNCTION__ << "segno_var" << segno_var_x << segno_var_y);

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

    one = &stroke->at(0);

    if(is_vertical){
        q = one->x();
    }
    else{
        q = one->y() - (one->x()) * m;
    }


    for(i = 0; i < len; i++){
        one = &stroke->at(i);

        is_near_line(m, precision, q, one);
    }

    WDebug(debug, __FUNCTION__ << qstr("Line precision: %1").arg(precision));

    return precision;
}

static void model_line_vertical(stroke *stroke, stroke_complex_line *data)
{
    const auto press = stroke->getPressure();
    const QRect FL = stroke->getFirstAndLast();
    QPointF TL = FL.topLeft();
    QPointF BR = FL.bottomRight();

    if(TL.y() > BR.y()){
        __swap(TL, BR);
    }

    const double x = TL.x();

    stroke->reset();

    data->topLeft = QPointF(x, TL.y());
    data->bottomRight = QPointF(x, BR.y());
    data->press = press;
}

static void model_line_generic(stroke *stroke, stroke_complex_line *data)
{
    const auto pressure = stroke->getPressure(0);
    data->topLeft  = stroke->at(0).toQPointF(1.);
    data->bottomRight = stroke->last().toQPointF(1.);

    if(data->topLeft.y() > data->bottomRight.y()){
        __swap(data->topLeft, data->bottomRight);
    }

    data->press = pressure;
    stroke->reset();
}

void model_line_create(stroke *stroke)
{
    W_ASSERT(stroke);
    bool &is_vertical = line_data.is_vertical;
    stroke_complex_line *data = (stroke_complex_line *)WMalloc(sizeof(stroke_complex_line));

    if(is_vertical){
        model_line_vertical(stroke, data);
    }else{
        model_line_generic(stroke, data);
    }

    stroke->reset();
    stroke->set_complex(stroke::COMPLEX_LINE, data);
}

void stroke_complex_line_append(stroke *stroke, const QPointF& point)
{
    auto *data = (stroke_complex_line *) stroke->get_complex_data();
    const auto dist1 = distance_not_square(data->topLeft, point);
    const auto dist2 = distance_not_square(data->bottomRight, point);

    if(dist1 > dist2){
        data->bottomRight = point;
    }else{
        data->topLeft = point;
    }
}

bool stroke_complex_is_inside_line(const stroke *stroke, const WLine &line, cdouble precision)
{
    const auto data = (const stroke_complex_line *)stroke->get_complex_data();

    W_ASSERT(stroke->is_line());

    WLine _line(data->topLeft, data->bottomRight);
    return _line.intersect(line, precision);
}

void stroke_complex_translate_line(stroke *stroke, const QPointF &offset)
{
    stroke_complex_line *data = (stroke_complex_line *)stroke->get_complex_data();
    data->topLeft += offset;
    data->bottomRight += offset;
}

static inline void stroke_complex_normal_line_generic(
        stroke              *_to,
        stroke_complex_line *data,
        int from, int to)
{
    double m, p;
    point_s point;
    const pressure_t press = data->press;

    W_ASSERT(from <= to);

    m =     (data->topLeft.y() - data->bottomRight.y()) /
            (data->topLeft.x() - data->bottomRight.x());

    p = data->topLeft.y() - data->topLeft.x() * m;

    for(; from <= to; from ++){
        const double x = (double(from) - p) / m;
        point._x = x;
        point._y = (double) from;
        _to->append(point, press);
    }
}

static inline void stroke_complex_normal_line_vertical(
        stroke              *_to,
        stroke_complex_line *data,
        int from, int to)
{
    point_s tmp;
    const pressure_t press = data->press;
    W_ASSERT(data->bottomRight.x() == data->topLeft.x());
    W_ASSERT(from <= to);

    tmp._x = data->topLeft.x();

    for(; from <= to; from ++){
        tmp._y = from;
        _to->append(tmp, press);
    }
}

void stroke_complex_make_normal_line   (const stroke *_from, stroke *_to)
{
    int from, to;
    stroke_complex_line *data = (stroke_complex_line *)_from->get_complex_data();
    W_ASSERT(_from->is_line());
    _to->reset();

    from    = (int) data->topLeft.y();
    to      = (int) data->bottomRight.y();

    W_ASSERT(from <= to);

    if(data->bottomRight.x() == data->topLeft.x()){
        stroke_complex_normal_line_vertical(_to, data, from, to);
    }else{
        stroke_complex_normal_line_generic(_to, data, from, to);
    }

    W_ASSERT(_to->is_normal());
}

bool stroke_complex_is_inside_line   (const stroke *_stroke, const QRectF &area, cdouble precision)
{
    const auto *data = (const stroke_complex_line *)_stroke->get_complex_data();
    W_ASSERT(_stroke->is_line());
    const auto rect = datastruct_rect(data->topLeft, data->bottomRight);
    WLine line(rect.topLeft(), rect.bottomRight());

    if(area.contains(data->topLeft))
        return true;
    if(area.contains(data->bottomRight))
        return true;

    return false;
}
