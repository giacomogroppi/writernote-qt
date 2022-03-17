#include "datastruct.h"
#include "log/log_ui/log_ui.h"
#include "utils/common_script.h"

static force_inline void append_point(const stroke &_from, stroke &_to, int from, int to)
{
    for(; from < to; from ++){
        _to.append(_from.at(from), _from.getPressure(from));
    }
}

// this function is usable only in this .o file
force_inline void datastruct::__changeId(int IndexPoint, stroke &__stroke, page &page, cbool threadSafe)
{
    stroke strokeToAppend;
    int lenPointInStroke;

    W_ASSERT(__stroke.is_normal());

    lenPointInStroke = __stroke.length();
    strokeToAppend.reset();

    WDebug(false, "datastruct::changeId start" << IndexPoint << __stroke.length()
             << __stroke.last()._x << __stroke.last()._y);

    append_point(__stroke, strokeToAppend, IndexPoint, lenPointInStroke);
    strokeToAppend.setMetadata(__stroke.getMetadata());

    if(threadSafe){
        pthread_mutex_lock(&_changeIdMutex);
    }

    // draw to old stroke with color_null
    page.drawForceColorStroke(__stroke, -1, COLOR_NULL, NULL);

    // we remove the point
    __stroke.removeAt(IndexPoint, __stroke.length() - 1);

    // we draw the new 2 stroke
    page.drawForceColorStroke(__stroke, -1, __stroke.getColor(1.0), NULL);
    page.drawForceColorStroke(strokeToAppend, -1, strokeToAppend.getColor(1.0), NULL);

    // we append the stroke
    page.append(strokeToAppend);

    if(threadSafe){
        pthread_mutex_unlock(&_changeIdMutex);
    }
}

void datastruct::changeId(int indexPoint, stroke &stroke, page &page)
{
    return __changeId(indexPoint, stroke, page, false);
}

void datastruct::changeIdThreadSave(int indexPoint, stroke &stroke, page &page)
{
    return __changeId(indexPoint, stroke, page, true);
}
