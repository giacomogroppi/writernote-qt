#include "../tabletcanvas.h"

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    if(data == NULL)
        return;

    int i, len;
    len = data->datatouch->m_point.length();
    //len = data->datatouch->x.length();
    if(len){
        for(i=0; i<len; i++)
            if(data->datatouch->m_point.at(i).idtratto == IDORIZZONALE)
                break;


        double delta;

        delta = data->datatouch->m_point.at(i+1).m_x;
        if(delta < width() && data->datatouch->m_point.at(i).m_x < 0){
            delta = data->datatouch->m_point.at(i).m_x;
            for(i=0; i<len; i++)
                data->datatouch->m_point.operator[](i).m_x += delta;
                //data->datatouch->x[i] += delta;
        }
    }

    initPixmap(true);
    this->isloading = true;
}
