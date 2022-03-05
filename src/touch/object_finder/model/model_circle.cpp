#include "touch/object_finder/model/model.h"
#include "touch/datastruct/stroke.h"
#include "utils/common_script.h"

static struct {
    double _x, _y, _r;
} circle_data;

void model_circle_create(stroke *stroke)
{

}

static void model_circle_precision(const QPointF &point, double &precision)
{
    double res;

    res = wPower(point.x(), 2);
}

double model_circle(const stroke *stroke)
{
    const auto area = stroke->getBiggerPointInStroke();
    double precision = 0.;
    int i, len;
    double &x = circle_data._x;
    double &y = circle_data._y;
    double &r = circle_data._r;

    len = stroke->length();

    r = (area.topLeft().y() - area.bottomRight().y()) / 2.;
    x = (area.topLeft().x() + area.bottomRight().x()) / 2.;
    y = (area.topLeft().y() + area.bottomRight().y()) / 2.;

    for(i = 0; i < len; i++){
        const auto &ref = stroke->at(i);
        model_circle_precision(ref.toQPointF(1.), precision);
    }

    return precision;
}
