#include "../tabletcanvas.h"
#include <QRect>
#include <QPainter>
#define GOMMASIZE 10

void TabletCanvas::gomma(QPainter &painter){
    int i, len, x, y;
    len = data->y.length();

    x = lastPoint.pos.x();
    y = lastPoint.pos.y();

    QPen pennatemp;
    pennatemp.setColor(Qt::white);
    painter.setPen(pennatemp);

    for(i=0; i<len; i++){
        if(x - GOMMASIZE < data->x.at(i) && x + GOMMASIZE > data->x.at(i)
                && y - GOMMASIZE < data->y.at(i)
                && y + GOMMASIZE > data->y.at(i)
                && data->idtratto.at(i) != -1){

            painter.drawPoint(data->x.at(i), data->y.at(i));
            data->removeat(i);
        }
    }

}
