#include "PenMethod.h"
#include "touch/dataTouch/stroke/StrokePre.h"

extern StrokePre __tmp;

PenMethod::PenMethod(std::function<int()> getSize, std::function<qint64 ()> getTime)
    :
      _getSize(getSize),
      _getTime(getTime)
{
}

bool PenMethod::touchBegin(const QPointF &point, Document &doc)
{

}

bool PenMethod::touchUpdate(const QPointF &point, Document &doc)
{

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

    return pageMod;
}
