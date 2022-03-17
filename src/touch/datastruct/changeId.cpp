#include "datastruct.h"
#include "log/log_ui/log_ui.h"
#include "utils/common_script.h"

// this function is usable only in this .o file
force_inline void datastruct::__changeId(int IndexPoint, stroke &__stroke, page &page, cbool threadSafe)
{
    stroke strokeToAppend;
    int lenPointInStroke;

    W_ASSERT(__stroke.is_normal());

    lenPointInStroke = __stroke.length();
    strokeToAppend.reset();

    WDebug(false, "datastruct::changeId start" << IndexPoint << __stroke.length()
             << __stroke.last()._x << __stroke.last()._y;)

    for(int secIndex = IndexPoint; secIndex < lenPointInStroke; secIndex ++){
        strokeToAppend.append(__stroke.at(secIndex), __stroke.getPressure());
    }

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

/*
 * the point at the IndexPoint
 * position is not included
 *
 * after this function you have to draw the image of
 * the page. Passing as a parameter all true
*/
void datastruct::changeId(int IndexPoint, int indexStroke, int indexPage)
{
    page &page = at_mod(indexPage);
    stroke &stroke = page.atStrokeMod(indexStroke);


    changeId(IndexPoint, stroke, at_mod(indexPage));
}

void datastruct::changeId(int indexPoint, stroke &stroke, page &page)
{
    return __changeId(indexPoint, stroke, page, true);
}

void datastruct::changeIdThreadSave(int indexPoint, stroke &stroke, page &page)
{
    return __changeId(indexPoint, stroke, page, true);
}
