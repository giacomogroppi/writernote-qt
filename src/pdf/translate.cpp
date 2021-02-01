#include "topdf.h"

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

    while(data->datatouch->zoom != 1.00){
        if(data->datatouch->zoom < 1.00)
            parent->m_canvas->zoomin(this->data);
        else
            parent->m_canvas->zoomon(this->data);
    }
}
