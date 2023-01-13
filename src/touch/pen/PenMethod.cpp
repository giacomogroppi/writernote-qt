#include "PenMethod.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/tabletcanvas.h"

#include <utility>

extern StrokePre __tmp;

PenMethod::PenMethod(std::function<pressure_t(double)> getSize,
                     std::function<int()> getTime,
                     QColor &color, QPen &pen) :
        _getSize(std::move(getSize)),
        _getTime(std::move(getTime)),
        _color(color),
        _pen(pen)
{
}

bool PenMethod::touchBegin(const QPointF &point, double size, Document &doc)
{

}

bool PenMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    uchar alfa;
    point_s tmp_point{};
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;
    const auto prop = doc.getZoom() == PROP_RESOLUTION ?
                      doc.getZoom() :
                      doc.getZoom() - .0000001;

    strokeTmp.setTime(static_cast<int>(
                              this->_getTime()
                      ));

    strokeTmp.setColor(this->_color);
    strokeTmp.setAlfaColor(alfa);

    tmp_point = point;
    pressure = this->_getSize(size);

    strokeTmp.append(tmp_point, pressure, (QPen &)_pen, prop);

    core::get_canvas()->_finder->move(point);
}

int PenMethod::touchEnd(const QPointF &, Document &doc)
{
    StrokePre & strokeToAppend = __tmp;
    int pageMod;
    qint64 time;
    const QPointF &PointFirstPage = doc.getPointFirstPage();

    if (un(strokeToAppend.isEmpty()))
        return -1;

    time = this->_getTime();

    /**
     * 2^31 ~ 2 Gs ~ 6.9 days. It's enough
     * */
    W_ASSERT(time < INT_MAX);

    strokeToAppend.adjust(PointFirstPage);

    std::shared_ptr<Stroke> res = strokeToAppend.merge();

    pageMod = doc.appendStroke(res);

    doc.at_mod(pageMod).triggerRenderImage(
            static_cast<int>(time),
            false
    );

    strokeToAppend = StrokePre();

    return pageMod;
}
