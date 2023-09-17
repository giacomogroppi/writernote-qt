#include "DataStruct.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

// this function is usable only in this .o file
force_inline void DataStruct::__changeId(int IndexPoint, Stroke &stroke, Page &page, cbool threadSafe)
{
    W_ASSERT(stroke.makeNormal() == nullptr);

    auto *strokeNormal = dynamic_cast<StrokeNormal *>(&stroke);

    if (threadSafe) {
        _changeIdMutex.lock();

        // draw to old stroke with color_null
        page.drawForceColorStroke(stroke, -1, COLOR_NULL, nullptr);

        _changeIdMutex.unlock();
    }

    SharedPtr<StrokeNormal> strokeToAppend = strokeNormal->split(IndexPoint);

    if(threadSafe){
        _changeIdMutex.lock();
    }

    // we draw the new 2 stroke
    page.drawForceColorStroke(stroke,           -1, stroke.getColor(1.0),           nullptr);
    page.drawForceColorStroke(*strokeToAppend,  -1, strokeToAppend->getColor(1.0),  nullptr);

    // we append the stroke
    page.append(strokeToAppend);

    if(threadSafe){
        _changeIdMutex.unlock();
    }
}

void DataStruct::changeIdThreadSave(int indexPoint, Stroke &stroke, Page &page)
{
    return __changeId(indexPoint, stroke, page, true);
}
