#include "touch/object_finder/model/model.h"
#include "testing/memtest.h"
#include <qmath.h>
#include "utils/WCommonScript.h"
#include "core/WLine.h"
#include "touch/dataTouch/stroke/StrokePre.h"

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
