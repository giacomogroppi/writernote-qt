#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/object_finder/model/model.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "utils/common_script.h"
#include "touch/dataTouch/stroke/stroke_complex_data.h"
#include "testing/memtest.h"
#include "qmath.h"
#include "core/WLine.h"

static stroke_complex_circle circle_data;
constexpr not_used bool debug = false;

static not_used void model_circle_print(const stroke_complex_circle *data)
{
    if(debug_enable()){
        qDebug() << "circle: " << data << data->_x << data->_y << data->_r << data->_press;
    }
}

void model_circle_create(Stroke *stroke)
{
    const auto press = stroke->getPressure();

    W_ASSERT(stroke);
    circle_data._press = press;

    stroke->reset();
    stroke_complex_circle *data = (stroke_complex_circle *)WMalloc(sizeof(stroke_complex_circle));
    memcpy(data, &circle_data, sizeof(circle_data));
    stroke->set_complex(Stroke::COMPLEX_CIRCLE, data);
}

static void model_circle_precision(const QPointF &point, double &precision)
{
    double res;
    const auto x = circle_data._x;
    const auto y = circle_data._y;

    /*
     * X^2 + Y^2 - R = res
     */

    res = wPower(point.x() - x, 2) +
          wPower(point.y() - y, 2)
          - circle_data._r;

    if(qAbs(res) > precision){
        precision = res;
    }
}

double model_circle(const StrokePre *stroke)
{
    const auto area = stroke->getBiggerPointInStroke();
    constexpr auto coef = 500.;
    constexpr auto _end = 10.;
    double precision = 0.;
    double &x = circle_data._x;
    double &y = circle_data._y;
    double &r = circle_data._r;

    {
        const auto rect = stroke->getFirstAndLast();
        if(!is_near(rect.topLeft(), rect.bottomRight(), _end)){
            WDebug(debug, "first point and last are not near" << rect.topLeft() << rect.bottomRight());
            return model_error;
        }
    }

    r = qAbs(area.topLeft().y() - area.bottomRight().y()) / 2.;
    x = (area.topLeft().x() + area.bottomRight().x()) / 2.;
    y = (area.topLeft().y() + area.bottomRight().y()) / 2.;

    W_ASSERT(r >= 0.);
    W_ASSERT(x >= 0.);
    W_ASSERT(y >= 0.);

    for (auto b = stroke->constBegin(); b != stroke->constEnd(); b ++) {
        model_circle_precision(*b, precision);
    }

    precision /= coef;

    WDebug(debug, qstr("Cricle precision: %1").arg(precision));

    return precision;
}

void stroke_complex_circle_append(Stroke *stroke, const QPointF& point)
{
    auto *data = (stroke_complex_circle *)stroke->get_complex_data();
    W_ASSERT(stroke->is_circle());

    data->_r = distance(QPointF(data->_x, data->_y), point);
}

static inline double distance_from_center(
        const stroke_complex_circle *data,
        const QPointF& point)
{
    return qSqrt(
        wPower(data->_x - point.x(), 2) +
        wPower(data->_y - point.y(), 2)
    );
}

static inline int is_internal(
        cdouble distance,
        cdouble precision,
        const stroke_complex_circle *data)
{
    const auto raggio = qSqrt(data->_r);
    const auto real_dist1 = qSqrt(distance);
    return real_dist1 <= raggio + precision;
}

// return true if the first distance is inside the circle,
// and the second not
static inline bool one_inside(
        cdouble                     _inside,
        cdouble                     _outside,
        const stroke_complex_circle *_data,
        cdouble                     prec)
{
    return is_internal(_inside, prec, _data) && !is_internal(_outside, prec, _data);
}

bool stroke_complex_is_inside_circle(const Stroke *stroke, const WLine &line, cdouble precision)
{
    /*
     * ci basta che un punto sia fuori dal cerchio,
     * e uno sia dentro
     */
    QPointF tl, br;
    const auto data = (const stroke_complex_circle *)stroke->get_complex_data();

    constexpr bool not_used debug = true;

    W_ASSERT(data->_x >= 0.);
    W_ASSERT(data->_r >= 0.);
    W_ASSERT(data->_y >= 0.);
    W_ASSERT(data->_press >= 0.);

    line.get_point(tl, br);

    cdouble distance1 = distance_from_center(data, tl);
    cdouble distance2 = distance_from_center(data, br);

    model_circle_print(data);
    WDebug(debug, distance1 << distance2 << tl << br);

    if(is_near(distance1, data->_r, precision) || is_near(distance2, data->_r, precision))
        return true;

    cbool res = one_inside(distance1, distance2, data, precision) ||
                one_inside(distance2, distance1, data, precision);

    return res;
}

void stroke_complex_translate_circle(Stroke *stroke, const QPointF &offset)
{
    stroke_complex_circle *data = (stroke_complex_circle *) stroke->get_complex_data();
    data->_x += offset.x();
    data->_y += offset.y();
}

static inline void append_to_stroke(
        Stroke                  *stroke,
        const QVector<point_s>  &point,
        const pressure_t        press)
{
    for(const auto &ref : qAsConst(point)){
        stroke->append(ref, press);
    }
}

void stroke_complex_make_normal_circle (const Stroke *_from, Stroke *_to)
{
    double from, to;
    point_s tmp;
    stroke_complex_circle *data;
    pressure_t press;
    QVector<point_s> _pointLeft, _pointRigth;
    _to->reset();

    W_ASSERT(_to->is_normal());
    W_ASSERT(_from->is_circle());

    data = (stroke_complex_circle *) _from->get_complex_data();
    press = data->_press;
    from =  data->_y - wPower(data->_r, 1);
    to =    data->_y + wPower(data->_r, 1);

    _pointLeft.reserve(to - from);
    _pointRigth.reserve(to - from);

    // from is the top of the circle
    W_ASSERT(from <= to);

    for(; from <= to;){
        const auto _res = wPower(double(from) - data->_y, 2);   // = y^2
        const double res1 = wPower(data->_r, 2) - _res;         // = x^2

        W_ASSERT(res1 >= 0.);

        const double res2 = qSqrt(res1);                        // = mod(x)

        tmp = point_s(data->_x + res2, from);                   // = x + radius
        _pointLeft.append(tmp);

        tmp.rx() = data->_x - res2;                               // = x - radius
        _pointRigth.insert(0, tmp);

        from += 1.;
    }

    append_to_stroke(_to, _pointLeft, press);
    append_to_stroke(_to, _pointRigth, press);
}

/*
 * Questa funzione viene chiamata se e solo se
 * il biggerData dello stroke è all'interno
 * dell'area in cui stiamo cercando.
 */
bool stroke_complex_is_inside_circle (const Stroke *_stroke, const QRectF &area, cdouble precision)
{
    const stroke_complex_circle *data = (stroke_complex_circle *)_stroke->get_complex_data();
    int internal = 0;

    W_ASSERT(_stroke->is_circle());
    W_ASSERT(area.intersects(_stroke->getBiggerPointInStroke()));
    W_ASSERT(area.topLeft().x() <= area.bottomRight().x());
    W_ASSERT(area.topLeft().y() <= area.bottomRight().y());

    if(area.contains(QPointF(data->_x, data->_y)))
        return true;

    const auto dist1 = distance_from_center(data, area.topLeft());
    const auto dist2 = distance_from_center(data, area.bottomLeft());
    const auto dist3 = distance_from_center(data, area.topRight());
    const auto dist4 = distance_from_center(data, area.bottomRight());

    internal += is_internal(dist1, precision, data);
    internal += is_internal(dist2, precision, data);
    internal += is_internal(dist3, precision, data);
    internal += is_internal(dist4, precision, data);

    return internal && internal < 4;
}
