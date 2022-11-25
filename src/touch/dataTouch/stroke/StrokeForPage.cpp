#include "StrokeForPage.h"
#include "touch/tabletcanvas.h"

StrokeForPage::StrokeForPage() = default;

void StrokeForPage::setPressure(pressure_t press)
{
    this->_data.force_pressure(press);
}

void StrokeForPage::append(const StrokeNormal &stroke)
{
    using namespace WCommonScript;
    auto &l = this->_data._point;

    for_each(stroke._point, [&l](const point_s &point) {
        l.append(point);
    });
}
