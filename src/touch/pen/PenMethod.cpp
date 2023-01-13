#include "PenMethod.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include "touch/tabletcanvas.h"
#include "touch/object_finder/object_finder.h"
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
    pressure_t pressure;
    StrokePre &strokeTmp = __tmp;

    const uchar alfa = 255;

    strokeTmp.setTime(_getTime());
    strokeTmp.setColor(_color);
    strokeTmp.setAlfaColor(alfa);

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, _pen, getProp(doc));
}

bool PenMethod::touchUpdate(const QPointF &point, double size, Document &doc)
{
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, (QPen &)_pen, getProp(doc));

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

double PenMethod::getProp(const Document &doc) const
{
    const auto prop = doc.getZoom() == PROP_RESOLUTION ?
                      doc.getZoom() :
                      doc.getZoom() - .0000001;
    return prop;
}
