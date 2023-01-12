#include "LaserMethod.h"
#include "core/core.h"
#include "touch/tabletcanvas.h"
#include "touch/laser/Laser.h"

extern StrokePre __tmp;

LaserMethod::LaserMethod(std::function<pressure_t()> getPress,
                         QPen &pen) :
    _getPress(std::move(getPress)),
    _pen(pen)
{
}

bool LaserMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
}

bool LaserMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    uchar alfa;
    point_s tmp_point{};
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;
    const auto prop = doc.getZoom() == PROP_RESOLUTION ?
                      doc.getZoom() :
                      doc.getZoom() - .0000001;

    strokeTmp.setTime(static_cast<int>(
        -1
    ));
    strokeTmp.setColor(Qt::red);
    strokeTmp.setAlfaColor(alfa);

    tmp_point = point;
    pressure = this->_getPress();
    strokeTmp.append(tmp_point, pressure, (QPen &)_pen, prop);
}

int LaserMethod::touchEnd(const QPointF &point, Document &doc)
{
    auto *canvas = core::get_canvas();

    if (un(__tmp.isEmpty()))
        return -1;

    canvas->_laser->append(__tmp);
    canvas->_laser->endMove();
    __tmp = StrokePre();

    return -1;
}
