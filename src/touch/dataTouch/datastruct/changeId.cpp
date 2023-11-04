#include "DataStruct.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

// this function is usable only in this .o file
force_inline void DataStruct::__changeId(int IndexPoint, Stroke &stroke, Page &page, cbool threadSafe)
{
    W_ASSERT(stroke.makeNormal() == nullptr);

    auto *strokeNormal = dynamic_cast<StrokeNormal *>(&stroke);

    if (threadSafe) {
        WMutexLocker guard(_changeIdMutex);

        // draw to old stroke with color_null
        page.drawForceColorStroke(stroke, AudioPosition::makeInvalid(), COLOR_NULL, nullptr);
    }

    SharedPtr<StrokeNormal> strokeToAppend = strokeNormal->split(IndexPoint);

    WMutexLocker guard(_changeIdMutex, (threadSafe) ?
        WMutexLocker<WMutex>::Lock :
        WMutexLocker<WMutex>::NotLock
    );

    // we draw the new 2 stroke
    page.drawForceColorStroke(stroke,           AudioPosition::makeInvalid(), stroke.getColor(1.0),           nullptr);
    page.drawForceColorStroke(*strokeToAppend,  AudioPosition::makeInvalid(), strokeToAppend->getColor(1.0),  nullptr);

    // we append the stroke
    page.append(strokeToAppend);
}

void DataStruct::changeIdThreadSave(int indexPoint, Stroke &stroke, Page &page)
{
    return __changeId(indexPoint, stroke, page, true);
}
