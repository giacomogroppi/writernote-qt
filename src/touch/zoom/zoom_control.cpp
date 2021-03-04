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

bool zoom_control::zoom(QPointF &point_translate, long double delta, int widthP, int maxWidth){
    //qDebug() << (double)data->zoom << " Delta -> " << (double) delta;

    /* TODO */
    if(delta < (long double)1){
        if(data->zoom - (1-delta) < (long double)0){
            return false;
        }
    }
    else{ /* data >= 1 */
        if(data->zoom + (delta-1) > (long double)2){
            return false;
        }
    }

    trasla(point_translate);

    unsigned int i, len;

    for(i=0, len = data->m_point.length(); i<len; i++){
        data->m_point.operator[](i).m_x *= delta;
        data->m_point.operator[](i).m_y *= delta;
    }

    trasla(point_translate, delta);

    delta = (delta >= (long double)1) ? (delta-1) : (-((long double)1)/delta+1);

    data->zoom += delta;

    return needToResize(
                widthP,
                maxWidth);
}

/*
 * function return true if we need to change the size of m_pixmap
*/
bool zoom_control::needToResize(int widthP, int maxWidth){

    /* if it's in max size possibile in the screen */
    if(widthP == maxWidth)
        return false;

    int i, len = data->m_point.length();
    for(i=0; i<len; i++)
        if(data->m_point.at(i).m_x > widthP && data->m_point.at(i).m_x < maxWidth)
            return true;


    return false;
}
