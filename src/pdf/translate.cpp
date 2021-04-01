#include "topdf.h"

#define ERRORE 0.01

void topdf::translate(){
    int i, len;

    double delta_y, delta_x;

    delta_y = data->datatouch->m_point.first().m_y;
    delta_x = data->datatouch->m_point.first().m_x;

    len = data->datatouch->posizionefoglio.length();

    for(i=0; i<len; i++)
        data->datatouch->posizionefoglio[i] += delta_y;

    /* move y */
    len = data->datatouch->m_point.length();
    for(i=0; i<len; i++)
        data->datatouch->m_point.operator[](i).m_y -= delta_y;

    /*
     * move x
     * the first point on the list is always initialized (0, 0)
    */
    QPointF(data->datatouch->m_point.first().m_x,
            data->datatouch->m_point.first().m_y);

    for(i=0; i<len; i++){
        data->datatouch->m_point.operator[](i).m_x -= delta_x;
    }

    /*
     * last data struct
    */
    /*
    len = data->datatouch->x.length();
    for(i=0; i<len; i++)
        data->datatouch->y[i] -= delta;

    for(i=0; i<len; i++){
        if(data->datatouch->x.at(i) < 0.0){
            delta = data->datatouch->x.at(i);

            for(k=0; k<len; k++)
                data->datatouch->x[i] += delta;
        }
    }*/

    /*
     * TODO -> adjust the zoom increase
     *         and fix this piece of code
    */
    long double temp = (long double)1 - data->datatouch->zoom;

    if(data->datatouch->zoom == (long double)1)
        return;

    QPointF temp_point = QPointF(1, 1);

    /*
     * we can pass any value as we are not interested in resizing
    */
    parent->m_canvas->zoom->zoom(temp_point,
                                 temp,
                                 0,
                                 0,
                                 0,
                                 0,
                                 data->datatouch);

}
