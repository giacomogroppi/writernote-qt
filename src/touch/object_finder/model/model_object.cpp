#include "model.h"
#include "touch/datastruct/datastruct.h"

constexpr double error = 5000;

struct{
    double m, q;
    bool is_vertical;
}line_data;

static force_inline not_used bool is_near(const QPointF& p1, const QPointF& p2, cint max)
{
    W_ASSERT(max >= 0);
    const QPointF tl = QPointF(p1.x() - max, p1.y() - max);
    const QPointF br = QPointF(p1.x() + max, p1.y() + max);
    return QRectF(tl, br).contains(p2);
}

static void is_near_line(cdouble m, double &max, cdouble q, const point_s *point)
{
    const auto x = point->x();
    const auto y = point->y();

    const double res = qAbs(x * m + q - y);

    if(res > max){
        max = res;
    }
}

static bool line_check_segno(cdouble segno, cdouble len)
{
    /*
     * se più del 5% dei punti presenta un "ritorno"
     * e è minore del 95 % allora non è una linea
    */
    return  (
                qAbs(segno) > len * .05 &&
                qAbs(segno) < len * .95
            );
}

double model_line(const stroke *stroke)
{
    int segno_var_x, segno_var_y;
    int i, len;
    const point_s *one, *two;
    const auto &area = stroke->getBiggerPointInStroke();

    double &m = line_data.m;
    double &q = line_data.q;
    double precision = 0;

    line_data.is_vertical = false;

    len = stroke->length();

    if(unlikely(!len)){
        return error;
    }

    {
        const double det = area.topRight().x() - area.bottomRight().x();

        if(unlikely(!det)){
            line_data.is_vertical = true;
            goto cont;
        }

        m = (area.topRight().y() - area.bottomRight().y()) / det;
    }

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

    if(line_check_segno(segno_var_x, len) || line_check_segno(segno_var_y, len))
        return error;

    if(segno_var_y < 0){
        /*
         * se segno_var_y è minore di 0 allora la retta che l'utente
         * ha disegnato va dall'alto a sinistra in basso a destra
        */
        m = 1. / m;
    }

    one = &stroke->at(0);

    q = one->y() - (one->x()) * m;

    for(i = 0; i < len; i++){
        one = &stroke->at(i);

        is_near_line(m, precision, q, one);
    }

    return precision;
}

double model_rect(const stroke *stroke)
{
    Q_UNUSED(stroke);
    return error;
}

double model_circle(const stroke *stroke)
{
    Q_UNUSED(stroke);
    return error;
}

void model_line_create(stroke *stroke){}
void model_rect_create(stroke *stroke){}
void model_circle_create(stroke *stroke){}
