#include "touch/object_finder/model/model.h"
#include "touch/datastruct/stroke.h"
#include "utils/common_script.h"
#include "touch/datastruct/stroke_complex_data.h"
#include "testing/memtest.h"
#include "qmath.h"
#include "core/wline.h"

static stroke_complex_circle circle_data;
constexpr not_used bool debug = false;

static not_used void model_circle_print(const stroke_complex_circle *data)
{
    if(debug_enable()){
        qDebug() << "circle: " << data << data->_x << data->_y << data->_r << data->_press;
    }
}

void model_circle_create(stroke *stroke)
{
    const auto press = stroke->getPressure();

    W_ASSERT(stroke);
    circle_data._press = press;

    stroke->reset();
    stroke_complex_circle *data = (stroke_complex_circle *)WMalloc(sizeof(stroke_complex_circle));
    memcpy(data, &circle_data, sizeof(circle_data));
    stroke->set_complex(stroke::COMPLEX_CIRCLE, data);
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

double model_circle(const stroke *stroke)
{
    const auto area = stroke->getBiggerPointInStroke();
    constexpr auto coef = 200.;
    constexpr auto _end = 10.;
    double precision = 0.;
    int i, len;
    double &x = circle_data._x;
    double &y = circle_data._y;
    double &r = circle_data._r;

    len = stroke->length();

    {
        const auto rect = stroke->getFirstAndLast();
        if(!is_near(rect.topLeft(), rect.bottomRight(), _end)){
            WDebug(debug, __func__ << "first point and last are not near" << rect.topLeft() << rect.bottomRight());
            return model_error;
        }
    }

    r = qAbs(area.topLeft().y() - area.bottomRight().y()) / 2.;
    x = (area.topLeft().x() + area.bottomRight().x()) / 2.;
    y = (area.topLeft().y() + area.bottomRight().y()) / 2.;

    W_ASSERT(r >= 0.);
    W_ASSERT(x >= 0.);
    W_ASSERT(y >= 0.);

    for(i = 0; i < len; i++){
        const auto &ref = stroke->at(i);
        model_circle_precision(ref.toQPointF(1.), precision);
    }

    precision /= coef;

    WDebug(debug, __FUNCTION__ << qstr("Cricle precision: %1").arg(precision));

    return precision;
}

void stroke_complex_circle_append(stroke *stroke, const QPointF& point)
{
    auto *data = (stroke_complex_circle *)stroke->get_complex_data();
    W_ASSERT(stroke->is_circle());

    data->_r = distance(QPointF(data->_x, data->_y), point);
}

bool stroke_complex_is_inside_circle(const stroke *stroke, const WLine &line, cdouble precision)
{
    /*
     * ci basta che un punto sia fuori dal cerchio,
     * e uno sia dentro
     */
    QPointF tl, br;
    const auto data = (const stroke_complex_circle *)stroke->get_complex_data();
    const auto prec = data->_press;
    constexpr not_used bool debug = true;

    W_ASSERT(data->_x >= 0.);
    W_ASSERT(data->_r >= 0.);
    W_ASSERT(data->_y >= 0.);
    W_ASSERT(data->_press >= 0.);

    line.get_point(tl, br);

    cdouble distance1 = qSqrt( wPower(data->_x - tl.x(), 2) + wPower(data->_y - tl.y(), 2));
    cdouble distance2 = qSqrt( wPower(data->_x - br.x(), 2) + wPower(data->_y - tl.y(), 2));

    model_circle_print(data);
    WDebug(debug, __FUNCTION__ << distance1 << distance2 << tl << br);

    if(is_near(distance1, data->_r, precision) || is_near(distance2, data->_r, precision))
        return true;

    cbool oneLess = distance1 <= data->_r - precision;
    cbool oneMore = distance1 <= data->_r + precision;

    cbool twoLess = distance2 <= data->_r - precision;
    cbool twoMore = distance2 <= data->_r + precision;

    return !!(oneLess ^ twoMore) || !!(oneMore ^ twoLess);
}

void stroke_complex_translate_circle(stroke *stroke, const QPointF &offset)
{
    stroke_complex_circle *data = (stroke_complex_circle *) stroke->get_complex_data();
    data->_x += offset.x();
    data->_y += offset.y();
}
