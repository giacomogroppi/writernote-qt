#include "touch/object_finder/model/model.h"
#include "touch/datastruct/datastruct.h"

constexpr double    error = 5000;
constexpr bool      debug = true;

struct{
    double m, q;
    bool is_vertical;
}line_data;

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

static bool line_check_segno(cdouble segno, cdouble len)
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

    qDebug() << "model_line" << qstr("%1%2").arg(one->y()).arg(one->x());

    for(i = 0; i < len; i++){
        one = &stroke->at(i);

        is_near_line(m, precision, q, one);
    }

    return precision;
}

static void model_line_vertical(stroke *stroke)
{
    int i;
    cint len = stroke->length();
    const QRect FL = stroke->getFirstAndLast();
    const QPointF &TL = FL.topLeft();
    const QPointF &BR = FL.bottomRight();

    const double x = TL.x();

    for(i = 0; i < len; i++){
        point_s &point = stroke->at_mod(i);

        if(point.y() < TL.y())
            point._y = TL.y();

        if(point.y() > BR.y())
            point._y = BR.y();

        point._x = x;

    }

}

static void model_line_generic(stroke *stroke)
{
    point_s point;
    const auto pressure = stroke->getPressure(0);
    const auto one  = stroke->at(0).toQPointF(1.);
    const auto last = stroke->last().toQPointF(1.);

    stroke->reset();

    point._x = one.x();
    point._y = one.y();
    stroke->append(point, pressure);

    point._x = last.x();
    point._y = last.y();
    stroke->append(point, pressure);
}

void model_line_create(stroke *stroke)
{
    W_ASSERT(stroke);
    bool &is_vertical = line_data.is_vertical;

    if(is_vertical){
        model_line_vertical(stroke);
    }else{
        model_line_generic(stroke);
    }
}
