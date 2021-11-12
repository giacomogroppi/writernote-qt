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
        /*if(translation.x() >= 0.0)
            translation.setX(0.0);
        if(translation.y() >= 0.0)
            translation.setY(0.0);*/
    }

    qDebug() << "translation" << translation << "max" << max;
    data->setPointFirstPage(translation);
}

bool zoom_control::zoom(QPointF &point_translate,
                        double delta,
                        const uint width,
                        const uint maxWidth,
                        const uint maxHeight,
                        const uint height,
                        datastruct *data){

    this->data = data;
    const QPointF pointRestore = point_translate * delta;
    const QPointF pointRiTranslate = pointRestore - point_translate;
    const bool max = width >= maxWidth;

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

    data->increaseZoom(delta, QSize(width, height));

    this->checkRespositioning(pointRiTranslate, max);
    //data->setPointFirstPage(pointRiTranslate);

    return !max || data->biggery() <= width;
}
