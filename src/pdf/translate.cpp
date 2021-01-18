#include "topdf.h"

void topdf::translate(){
    int i, len;

    int delta, k;

    delta = data->posizionefoglio.first() < 0;
    len = data->posizionefoglio.length();
    for(i=0; i<len; i++)
        data->posizionefoglio[i] += delta;


    len = data->x.length();
    for(i=0; i<len; i++)
        data->y[i] += delta;

    for(i=0; i<len; i++){
        if(data->x.at(i) < 0.0){
            delta = data->x.at(i);

            for(k=0; k<len; k++)
                data->x[i] += delta;
        }
    }

    while(data->zoom != 1.00){
        if(data->zoom < 1.00)
            parent->m_canvas->zoomin(this->data);
        else
            parent->m_canvas->zoomon(this->data);
    }
}
