#include "zoom_control.h"
#include <QDebug>
#include "touch/event/itspossibletoscroll.h"
#include "touch/datastruct/datastruct.h"
#include "touch/event/itspossibletoscroll.h"

/*
 * if delta > 1 we are zoom in
*/

void zoom_control::checkRespositioning(const QPointF &f, const bool max)
{
    QPointF translation(f);
    if(!max){
        translation = QPointF(0.0, 0.0);
    }

    qDebug() << "translation" << translation << "max" << max;
    data->setPointFirstPage(translation);
}

bool zoom_control::zoom(QPointF &pointMiddle, double delta, bool &zoomChange,
                        const QSize &sizePixmap, const QSize &maxSize, datastruct *data){

    this->data = data;
    const QPointF pointRiTranslate = pointMiddle * (delta - double(1.0));
    const bool max = sizePixmap.width() >= maxSize.width();

    const double zoom = data->getZoom();
    //qDebug() << "Current zoom " << zoom << " delta " << delta;

    {
        const double newPossibleZoom = zoom + delta - double(1.0);
        if(!datastruct::isOkZoom(newPossibleZoom))
            return false;
    }

    delta = delta - double(1.0);

    zoomChange = true;

    this->checkRespositioning(pointRiTranslate, max);

    data->increaseZoom(delta, sizePixmap);
    return !max || data->biggery() <= sizePixmap.width();
}
