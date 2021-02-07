#include "topdf.h"

#define ERRORE 0.01

void topdf::translate(){
    int i, len;

    int delta, k;

    delta = data->datatouch->posizionefoglio.first();
    len = data->datatouch->posizionefoglio.length();
    for(i=0; i<len; i++)
        data->datatouch->posizionefoglio[i] += delta;

    len = data->datatouch->x.length();
    for(i=0; i<len; i++)
        data->datatouch->y[i] -= delta;

    for(i=0; i<len; i++){
        if(data->datatouch->x.at(i) < 0.0){
            delta = data->datatouch->x.at(i);

            for(k=0; k<len; k++)
                data->datatouch->x[i] += delta;
        }
    }

    long double temp = (long double)1 - data->datatouch->zoom;

    if(data->datatouch->zoom == (long double)1)
        return;

    QPointF temp_point = QPointF(1, 1);

    parent->m_canvas->zoom->zoom(temp_point, temp);

}
