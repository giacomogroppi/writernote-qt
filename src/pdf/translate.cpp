#include "topdf.h"

void topdf::adjastTranslation(Document *doc, const int width_pdf,
                              const int height_pdf)
{
    uint i;
    const int width_doc = doc->datatouch->currentWidth();
    const int height_doc = doc->datatouch->currentHeight();
    const double delta = double(width_doc * height_pdf) / double(height_doc*width_pdf);

    const uint len = doc->datatouch->length();
    for(i=0; i<len; ++i){
        doc->datatouch->at_mod(i)->m_y *= delta;
    }
}

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
