#include "topdf.h"

#define ERRORE 0.01

void topdf::translate(frompdf *m_pdf){
    data->datatouch->scala_all(m_pdf);

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
                                 data->datatouch,
                                 m_pdf);

}
