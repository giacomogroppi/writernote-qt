#include "StrokeLineGenerator.h"
#include "StrokePre.h"
#include "StrokeLine.h"
#include "StrokeNormal.h"
#include "StrokeComplexCommon.h"

static struct{
    double m, q;
    bool is_vertical;
} line_data;

void StrokeLineGenerator::is_near_line(double m, double &max, cdouble q, const PointF *point)
{
    const auto x = point->x();
    const auto y = point->y();
    double res;

    if (line_data.is_vertical) {
        res = std::abs(q - x);
        if (res > max) {
            max = res;
        }
    } else {
        res = std::abs(x * m + q - y);
        if (res > max) {
            max = res;
        }
    }
}

void StrokeLineGenerator::makeVertical(const WListFast<PointF>& points,
                                       const WListFast<pressure_t>& pressures,
                                       const RectF& area, StrokeLine &res)
{
    using namespace WCommonScript;

    // TODO: make this an average
    const auto press = pressures.first();

    const RectF FL {
         points.first(),
         points.last()
    };

    PointF TL = FL.topLeft();
    PointF BR = FL.bottomRight();

    if (TL.y() > BR.y()) {
        swap(TL, BR);
    }

    const double x = TL.x();

    res._data.pt1 = PointF(x, TL.y());
    res._data.pt2 = PointF(x, BR.y());
    res._data.press = press;
}

void StrokeLineGenerator::makeGeneric(const WListFast<PointF>& points,
                                      const WListFast<pressure_t>& pressures,
                                      const RectF& area, StrokeLine &res)
{
    using namespace WCommonScript;

    // make the pressure an average
    const auto pressure = pressures.first();
    res._data.pt1 = points.first();
    res._data.pt2 = points.last();

    if (res._data.pt1.y() > res._data.pt2.y()) {
        swap(res._data.pt1, res._data.pt2);
    }

    res._data.press = pressure;
}

std::unique_ptr<Stroke> StrokeLineGenerator::make(const WListFast<PointF>& points,
                                                  const WListFast<pressure_t>& pressures,
                                                  const RectF& area)
{
    std::unique_ptr<StrokeLine> tmp(new StrokeLine);

    if (line_data.is_vertical) {
        StrokeLineGenerator::makeVertical(points, pressures, area, *tmp);
    } else {
        StrokeLineGenerator::makeGeneric(points, pressures, area, *tmp);
    }

    // TODO: make this an average
    tmp->_data.press = pressures.first();

    return tmp;
}

force_inline void StrokeLineGenerator::model_line_adjust_m(double &m)
{
    const double mod = std::abs(m);
    if (mod < 0.07) {
        m = 0;
    }
    if (mod > 25.) {
        line_data.is_vertical = true;
    }
}

double StrokeLineGenerator::model_near(const WListFast<PointF> &points, const WListFast<pressure_t> &pressures,
                                       const RectF &area) noexcept
{
    int segno_var_x, segno_var_y;
    const PointF *one, *two;
    auto b = points.constBegin();
    const auto e = points.constEnd();

    double &m = line_data.m;
    double &q = line_data.q;
    bool &is_vertical = line_data.is_vertical;

    double precision = 0;

    line_data.is_vertical = false;

    /** list empty */
    if (b == e) {
        return StrokeComplexCommon::error;
    }

    //WDebug(debug, __FUNCTION__ << area.topLeft() << area.bottomRight());

    {
        const double det = area.topLeft().x() - area.bottomRight().x();

        if(!det){
            WDebug(StrokeLineGeneratorDebug, "det = 0");
            is_vertical = true;
            goto cont;
        }

        m = (area.topLeft().y() - area.bottomRight().y()) / det;
    }

    model_line_adjust_m(m);

    WDebug(StrokeLineGeneratorDebug, qstr("m: %1 q: %2 isVertical %3").arg(m).arg(q).arg(line_data.is_vertical));

cont:
    segno_var_x = segno_var_y = 0;

    one = & (*b);
    for (b ++; b != e; b ++) {
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

    WDebug(StrokeLineGeneratorDebug, "segno_var" << segno_var_x << segno_var_y);

    if (segno_var_y < 0) {
        /**
         * se segno_var_y è minore di 0 allora la retta che l'utente
         * ha disegnato va dall'alto a sinistra in basso a destra
        */
        m = 1. / m;
    }

    one = & (*points.constBegin());

    if (is_vertical) {
        q = one->x();
    } else {
        q = one->y() - (one->x()) * m;
    }

    for( b = points.constBegin(); b != e; b++ ){
        one = & (*b);

        is_near_line(m, precision, q, one);
    }

    WDebug(StrokeLineGeneratorDebug, qstr("Line precision: %1").arg(precision));

    return precision;
}
