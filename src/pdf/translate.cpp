#include "topdf.h"

void topdf::translate(){
    int i, len;
    len = this->data->x.length();

    while(data->posizionefoglio.first() < 0){
        len = data->posizionefoglio.length();
        for(i=0; i<len; i++){
            data->posizionefoglio[i] += 1;
        }

        len = data->x.length();
        for(i=0; i<len; i++){
            data->y[i] += 1;
        }
    }

    while(data->zoom != 1.00){
        if(data->zoom < 1.00)
            parent->m_canvas->zoomin(this->data);
        else
            parent->m_canvas->zoomon(this->data);
    }
}
