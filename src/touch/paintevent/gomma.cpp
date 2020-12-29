#include "../tabletcanvas.h"
#include <QRect>
#include <QPainter>
#include "../method/methoddefinition.h"

void TabletCanvas::gomma(QPainter &painter){
    int i, len, x, y, id, temp, k;
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
            if(data->needtochangeid(i)){
                /* se il punto è in mezzo alla lista deve cambiare l'id di tutto quello che viene dopo */
                id = data->idtratto.last() + 1;
                temp = data->idtratto.at(i);
                for(k=0; data->idtratto.at(i) == temp; k++)
                    data->idtratto[k] = id;
            }

            data->removeat(i);
        }
    }

}
