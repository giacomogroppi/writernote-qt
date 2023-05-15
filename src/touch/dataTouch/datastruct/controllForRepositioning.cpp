#include "DataStruct.h"

/**
 * this function only manage the case in which
 * the first point is positive
*/
void DataStruct::controllForRepositioning(PointF &translateTo)
{
    if (isempty_touch())
        return;

    const auto point = this->getPointFirstPage();

    translateTo.setX((point.x() > 0) ? point.x() : 0.0);
    translateTo.setY((point.y() > 0) ? point.y() : 0.0);

    DataStruct::inverso(translateTo);
}
