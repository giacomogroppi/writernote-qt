#include "zoom_control.h"

zoom_control::zoom_control()
{

}

void zoom_control::trasla(QPointF &point_translate, long double delta){
    int i, len;
    len = data->x.length();

    if(point_translate == QPointF(0, 0))
        return;

    long double x, y;
    x = point_translate.x();
    y = point_translate.y();

    if(delta){
        x *= (-1*delta);
        y *= (-1*delta);
    }

    for(i=0; i<len; i++){
        data->x[i] -= x;
        data->y[i] -= y;
    }

}

void zoom_control::zoom(QPointF &point_translate, long double delta){
    trasla(point_translate);

    int i, len;

    for(i=0, len = data->x.length(); i<len; i++){
        data->x[i] *= delta;
        data->y[i] *= delta;
    }

    data->zoom += delta/((long double)10);

    trasla(point_translate, delta);
}
