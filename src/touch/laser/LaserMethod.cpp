#include "LaserMethod.h"
#include "core/core.h"
#include "touch/tabletcanvas.h"
#include "touch/laser/Laser.h"

extern StrokePre __tmp;

LaserMethod::LaserMethod(std::function<pressure_t()> getPress) :
    _getPress(std::move(getPress))
{
}

bool LaserMethod::touchBegin(const QPointF &point, double size, Document &doc)
{
}

bool LaserMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    double size;
    uchar alfa;
    point_s tmp_point{};
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;
    const QPointF &pointTouch = event->position();
    const auto *data = this->getDoc();
    const auto prop = data->getZoom() == PROP_RESOLUTION ?
                      data->getZoom() :
                      data->getZoom() - .0000001;

    size = this->_getPress();
    strokeTmp.setTime(static_cast<int>(
        -1
    ));
    strokeTmp.setColor(Qt::red);
    strokeTmp.setAlfaColor(alfa);


    tmp_point = pointTouch;
    pressure = un(highlighter) ? _highlighter->getSize(size) : _pen_ui->getSize(size);

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
