#include "datastruct.h"
#include "utils/WCommonScript.h"

static force_inline void append_point(const Stroke &_from, Stroke &_to, int from, int to)
{
    for(; from < to; from ++){
        _to.append(_from.at(from), _from.getPressure(from));
    }
}

// this function is usable only in this .o file
force_inline void datastruct::__changeId(int IndexPoint, Stroke &__stroke, Page &page, cbool threadSafe)
{
    Stroke strokeToAppend;
    int lenPointInStroke;

    W_ASSERT(__stroke.is_normal());

    lenPointInStroke = __stroke.length();
    strokeToAppend.reset();

    WDebug(false, "start" << IndexPoint << __stroke.length()
             << __stroke.last());

    append_point(__stroke, strokeToAppend, IndexPoint, lenPointInStroke);
    strokeToAppend.setMetadata(__stroke.getMetadata());

    if(threadSafe){
        _changeIdMutex.lock();
    }

    // draw to old stroke with color_null
    page.drawForceColorStroke(__stroke, -1, COLOR_NULL, nullptr);

    // we remove the point
    __stroke.removeAt(IndexPoint, __stroke.length() - 1);

    // we draw the new 2 stroke
    page.drawForceColorStroke(__stroke, -1, __stroke.getColor(1.0), nullptr);
    page.drawForceColorStroke(strokeToAppend, -1, strokeToAppend.getColor(1.0), nullptr);

    // we append the stroke
    page.append(strokeToAppend);

    if(threadSafe){
        _changeIdMutex.unlock();
    }
}

void datastruct::changeIdThreadSave(int indexPoint, Stroke &stroke, Page &page)
{
    return __changeId(indexPoint, stroke, page, true);
}
