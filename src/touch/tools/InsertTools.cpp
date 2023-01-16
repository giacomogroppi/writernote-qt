#include "InsertTools.h"
#include "touch/tabletcanvas.h"
#include "touch/object_finder/object_finder.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include <utility>

extern StrokePre __tmp;

InsertTools::InsertTools(std::function<int()> getTime,
                         std::function<pressure_t(double)> getSize,
                         QColor &color,
                         QPen &pen) :
                         _getTime(std::move(getTime)),
                         _getSize(std::move(getSize)),
                         _color(color),
                         _pen(pen)
{
}

bool InsertTools::touchBegin(const QPointF &point, double size, class Document &doc)
{
    pressure_t pressure;
    StrokePre &strokeTmp = __tmp;

    W_ASSERT(__tmp.isEmpty());

    strokeTmp.setTime(_getTime());
    strokeTmp.setColor(_color);
    strokeTmp.setAlfaColor(getAlfa());

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, _pen, getProp(doc));

    return true;
}

bool InsertTools::touchUpdate(const QPointF &point, double size, class Document &doc)
{
    StrokePre &strokeTmp = __tmp;
    pressure_t pressure;

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, (QPen &)_pen, getProp(doc));

    core::get_canvas()->_finder->move(point);

    return true;
}

int InsertTools::touchEnd(const QPointF &, class Document &doc)
{
    StrokePre & strokeToAppend = __tmp;
    int pageMod;
    const QPointF &PointFirstPage = doc.getPointFirstPage();

    if (un(strokeToAppend.isEmpty()))
        return -1;

    strokeToAppend.adjust(PointFirstPage);

    std::shared_ptr<Stroke> res = strokeToAppend.merge();

    pageMod = doc.appendStroke(res);

    doc.at_mod(pageMod).triggerRenderImage(
            -1,
            false
    );

    strokeToAppend = StrokePre();

    W_ASSERT(__tmp.isEmpty());

    return pageMod;
}
