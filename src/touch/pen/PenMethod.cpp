#include "PenMethod.h"

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

int PenMethod::touchEnd(const QPointF &point, Document &doc)
{

}
