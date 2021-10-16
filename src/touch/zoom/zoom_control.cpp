#include "zoom_control.h"
#include <QDebug>
#include "../event/itspossibletoscroll.h"
#include "../datastruct/datastruct.h"

void zoom_control::trasla(QPointF point_translate,
                          datastruct *data,
                          long double delta){
    if(point_translate == QPointF(0.0, 0.0))
        return;

    if(!delta){
        datastruct::inverso(point_translate);
    }

    data->scala_all(point_translate);

}

/*
 * if delta > 1 we are zoom in
*/

bool zoom_control::zoom(QPointF &point_translate,
                        double delta,
                        const uint width,
                        const uint maxWidth,
                        const uint height,
                        const uint maxHeight,
                        datastruct *data){
    uint i, len;
    Q_UNUSED(maxHeight);
    
    /* TODO */
    if(delta < (long double)1){
        if(data->zoom - (1.0-delta) < (long double)0){
            return false;
        }
    }
    else{ /* data >= 1 */
        if(data->zoom + (delta-1.0) > (long double)2){
            return false;
        }
    }

    trasla(point_translate, data);

    

    len = data->length();

    for(i=0; i<len; ++i){
        data->at_mod(i)->m_x *= delta;
        data->at_mod(i)->m_y *= delta;
    }

    trasla(point_translate, data, delta);

    delta = (delta >= (double)1) ? (delta-1) : (-((double)1)/delta+1);

    data->zoom += delta;

    data->adjustHeight(height, true);

    if(width == maxWidth)
        return false;

    return data->adjustAll(height, width);
}
