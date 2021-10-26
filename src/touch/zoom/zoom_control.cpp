#include "zoom_control.h"
#include <QDebug>
#include "../event/itspossibletoscroll.h"
#include "../datastruct/datastruct.h"

/*
 * if delta > 1 we are zoom in
*/

bool zoom_control::zoom(QPointF &point_translate,
                        double delta,
                        const uint width,
                        const uint maxWidth,
                        const uint maxHeight,
                        const uint height,
                        datastruct *data){
    /*Q_UNUSED(point_translate);
    Q_UNUSED(delta);
    Q_UNUSED(width);
    Q_UNUSED(maxHeight);
    Q_UNUSED(maxWidth);
    Q_UNUSED(height);
    Q_UNUSED(data);*/
    //uint i;
    //const uint lenPage = data->lengthPage();
    qDebug() << "zoom call";
    Q_UNUSED(maxHeight)
    const QPointF pointRestore = point_translate * delta;
    const QPointF pointRiTranslate = pointRestore - point_translate;

    if(delta < 1.00){
        if(data->zoom - (1.0-delta) < (long double)0){
            return false;
        }
    }
    else{
        if(data->zoom + (delta-1.00) > 2.00){
            return false;
        }
    }

    delta = (delta >= 1.00) ? (delta-1.0) : (-(1.00)/delta+1.00);
    qDebug() << "zoom --> delta " << delta;
    data->zoom += delta;

    data->setPointFirstPage(pointRiTranslate);

    data->adjustAll(width, height);

    if(width == maxWidth)
        return false;

    data->adjustAll(height, width);
    return true;
}
