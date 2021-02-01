#include "draw_image.h"
#include <QRect>

void draw_image(currenttitle_class *data, QPainter *painter){
    int i, len;

    len = data->datatouch->immagini.length();
    for(i=0; i<len; i++){
        painter->drawImage(QRect(
                               data->datatouch->immagini.at(i).i,
                               data->datatouch->immagini.at(i).f),
                           data->datatouch->immagini.at(i).immagini);
    }


}
