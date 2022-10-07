#include "zoom_control.h"
#include <QDebug>
#include "touch/event/itspossibletoscroll.h"
#include "currenttitle/document.h"

/**
 * if delta > 1 we are zoom in
*/

void zoom_control::checkRespositioning(const QPointF &f, cbool max)
{
    QPointF translation(f);

    if(!max){
        translation = QPointF(0., 0.);
    }

    datastruct::inverso(translation);

    WDebug(false, "translation" << translation << "max" << max);

    const auto PointFirstPage = data->getPointFirstPageNoZoom();
    const auto tmp = PointFirstPage + translation;

    if(tmp.x() > 0.){
        translation.setX(0.);
    }
    if(tmp.y() > 0.){
        translation.setY(0.);
    }

    data->scala_all(translation, -1);
}

// delta < 1. we ara decreasing zoom
bool zoom_control::zoom(const QPointF   &pointMiddle,
                        double          delta,
                        bool            &zoomChange,
                        const QSize     &sizePixmap,
                        const QSize     &maxSize,
                        Document      *data)
{
    this->data = data;

    W_ASSERT(pointMiddle.x() >= 0. && pointMiddle.y() >= 0.);
    QPointF diff;
    {
        const QPointF newPoint = pointMiddle * (delta);
        diff.setX( newPoint.x() - pointMiddle.x() );
        diff.setY( newPoint.y() - pointMiddle.y() );
    }

    cbool max = sizePixmap.width() >= maxSize.width(); // false if it's max
    const double zoom = data->getZoom();

    //qDebug() << "Current zoom " << zoom << " delta " << delta;

    {
        const double newPossibleZoom = zoom + delta - 1.;
        if(!datastruct::isOkZoom(newPossibleZoom))
            return false;
    }

    delta = delta - 1.;

    zoomChange = true;

    this->checkRespositioning(diff, max);

    data->increaseZoom(delta, sizePixmap);

    if(!max){
        return true;
    }

    return data->biggerx() <= sizePixmap.width();
}
