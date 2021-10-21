#include "topdf.h"

void topdf::translate(){
    data->datatouch->scala_all();
    return;
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
