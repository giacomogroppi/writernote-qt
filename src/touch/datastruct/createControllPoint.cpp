#include "datastruct.h"

static size_t data;
int i;

size_t point_s::createControll() const{
    data = 0;
    data += m_x;
    data += m_y;
    data += m_pressure;
    data += idtratto;

    for(i=0; i<NCOLOR; ++i){
        data += m_color.colore[i];
    }

    data += this->rotation;
    data += this->m_posizioneaudio;

    return data;
}
