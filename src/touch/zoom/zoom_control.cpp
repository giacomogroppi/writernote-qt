#include "zoom_control.h"
#include <QDebug>

zoom_control::zoom_control()
{

}

void zoom_control::trasla(QPointF &point_translate, long double delta){
    unsigned int i, len;
    len = data->m_point.length();

    if(point_translate == QPointF(0, 0))
        return;

    double x, y;
    x = point_translate.x();
    y = point_translate.y();

    if(delta){
        x *= (-1*delta);
        y *= (-1*delta);
    }

    for(i=0; i<len; i++){
        data->m_point.operator[](i).m_x -= x;
        data->m_point.operator[](i).m_y -= y;
    }

    /*
     * last data struct
    */
    /*for(i=0; i<len; i++){
        data->x[i] -= x;
        data->y[i] -= y;
    }*/

}

#define D 2

void zoom_control::zoom(QPointF &point_translate, long double delta){
    //qDebug() << (double)data->zoom << " Delta -> " << (double) delta;

    /* TODO */
    if(delta < (long double)1){
        if(data->zoom - (1-delta) < (long double)0){
            return;
        }
    }
    else{ /* data >= 1 */
        if(data->zoom + (delta-1) > (long double)2){
            return;
        }
    }

    trasla(point_translate);

    unsigned int i, len;

    for(i=0, len = data->m_point.length(); i<len; i++){
        data->m_point.operator[](i).m_x *= delta;
        data->m_point.operator[](i).m_y *= delta;
    }

    /*
     * last data struct
    */
    /*
    for(i=0, len = data->x.length(); i<len; i++){
        data->x[i] *= delta;
        data->y[i] *= delta;
    }*/

    trasla(point_translate, delta);

    delta = (delta >= (long double)1) ? (delta-1) : (-((long double)1)/delta+1);

    data->zoom += delta;
}
