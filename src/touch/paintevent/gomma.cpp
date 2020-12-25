#include "../tabletcanvas.h"

#include <QPainter>
#define GOMMASIZE 10

void TabletCanvas::gomma(QPainter &painter){
    int i, len, x, y;
    len = data->y.length();

    x = lastPoint.pos.x();
    y = lastPoint.pos.y();

    for(i=0; i<len; i++){
        if(x - GOMMASIZE < data->x.at(i) && x + GOMMASIZE > data->x.at(i)
                && y - GOMMASIZE < data->y.at(i)
                && y + GOMMASIZE > data->y.at(i)){
            painter.drawRect(x-GOMMASIZE, y-GOMMASIZE,
                             x+GOMMASIZE, y+GOMMASIZE);
            data->removeat(i);
        }
    }

}
