#include "InsertTools.h"
#include "touch/TabletUtils.h"
#include "touch/object_finder/ObjectFinder.h"
#include "touch/dataTouch/stroke/StrokePre.h"
#include <utility>

extern StrokePre *__tmp;

InsertTools::InsertTools(std::function<int()> getTime,
                         std::function<pressure_t(double)> getSize,
                         std::function<void(const PointF &)> objectMove,
                         WColor &color,
                         WPen &pen)
    : _getTime(std::move(getTime))
    ,  _getSize(std::move(getSize))
    ,  _objectMove(std::move(objectMove))
    ,  _color(color)
    ,  _pen(pen)
{
}

auto InsertTools::touchBegin(const PointF &point, double size, class Document &doc) -> UpdateEvent
{
    pressure_t pressure;
    StrokePre &strokeTmp = *__tmp;

    W_ASSERT(strokeTmp.isEmpty());

    strokeTmp.reset_img();
    strokeTmp.setTime(_getTime());
    strokeTmp.setColor(_color);
    strokeTmp.setAlfaColor(getAlfa());

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, _pen, getProp(doc));

    // TODO: adjust [maybe]
    return UpdateEvent::makeStroke();
}

auto InsertTools::touchUpdate(const PointF &point, double size, class Document &doc) -> UpdateEvent
{
    StrokePre &strokeTmp = *__tmp;
    pressure_t pressure;

    pressure = this->_getSize(size);

    strokeTmp.append(point, pressure, (WPen &)_pen, getProp(doc));

    this->_objectMove(point);

    // TODO: adjust [maybe]
    return UpdateEvent::makeStroke();
}

auto InsertTools::touchEnd(const PointF &, class Document &doc) -> UpdateEvent
{
    StrokePre & strokeToAppend = *__tmp;
    int pageMod;
    const PointF &PointFirstPage = doc.getPointFirstPage();

    if (un(strokeToAppend.isEmpty()))
        return UpdateEvent::makeSheet();

    strokeToAppend.adjust(PointFirstPage);

    std::shared_ptr<Stroke> res = strokeToAppend.merge();

    pageMod = doc.appendStroke(std::move(res));

    doc.at_mod(pageMod).triggerRenderImage(
            -1,
            false
    );

    strokeToAppend = StrokePre();

    W_ASSERT(__tmp->isEmpty());

    return UpdateEvent::makeStroke() | UpdateEvent::makePage(pageMod, pageMod + 1);
}
