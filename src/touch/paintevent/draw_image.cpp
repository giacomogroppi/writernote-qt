#include "draw_image.h"
#include <QRect>

void draw_image(currenttitle_class *data, QPainter *painter){
    int i, len;

    len = data->immagini.length();
    for(i=0; i<len; i++){
        painter->drawImage(QRect(
                               data->immagini.at(i).i,
                               data->immagini.at(i).f),
                           data->immagini.at(i).immagini);
    }


}
