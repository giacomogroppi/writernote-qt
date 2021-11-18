#include "zoom_control.h"
#include <QDebug>
#include "../event/itspossibletoscroll.h"
#include "../datastruct/datastruct.h"
#include "../event/itspossibletoscroll.h"

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

    //delta = (delta >= 1.00) ? (delta-1.0) : (-(1.00)/delta+1.00);

    delta = delta - double(1.0);

    //qDebug() << "New delta" << delta;

    this->checkRespositioning(pointRiTranslate, max);

    data->increaseZoom(delta, sizePixmap);
    return !max || data->biggery() <= sizePixmap.width();
}
