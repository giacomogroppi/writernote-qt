#include "../tabletcanvas.h"
#include <QRect>
#include <QPainter>
#include "../method/methoddefinition.h"

static inline bool isin(int , int , int, int , int);

void TabletCanvas::gomma(QPainter &painter){
    int i, len, x, y, id, temp, k;
    len = data->datatouch->y.length();

    x = lastPoint.pos.x();
    y = lastPoint.pos.y();

    if(this->gomma_cancella){
        for(i=0; i<len; i++){
            if(isin(data->datatouch->x.at(i),
                    data->datatouch->y.at(i),
                    x,
                    y,
                    data->datatouch->idtratto[i])){

                id = data->datatouch->idtratto[i];

                for(; i<len; i++){
                    if(data->datatouch->idtratto[i] != id)
                        break;

                    /* decreases alfa */
                    data->datatouch->color[i].colore[3] /= 2;

                }
                i--;
            }
        }
    }
    else{
        QPen pennatemp;
        pennatemp.setColor(Qt::white);
        pennatemp.setWidth(GOMMASIZE);
        for(i=0; i<len; i++){
            if(isin(data->datatouch->x[i],
                    data->datatouch->y[i],
                    x,
                    y,
                    data->datatouch->idtratto[i])){

                painter.drawPoint(data->datatouch->x.at(i), data->datatouch->y.at(i));

                if(data->datatouch->needtochangeid(i)){
                    /* se il punto è in mezzo alla lista deve cambiare l'id di tutto quello che viene dopo */
                    id = data->datatouch->idtratto.last() + 1;
                    temp = data->datatouch->idtratto.at(i);
                    for(k=0; data->datatouch->idtratto.at(i) == temp; k++)
                        data->datatouch->idtratto[k] = id;
                }

                data->datatouch->removeat(i);
            }
        }
    }

}

void TabletCanvas::gomma_delete(){
    if(gomma_delete_id.isEmpty())
        return;

    int i, k, len, len_cancella;

    len = data->datatouch->x.length();
    len_cancella = gomma_delete_id.length();

    for(k=0; k<len_cancella; k++){
        for(i=0; i<len; i++){
            if(data->datatouch->idtratto.at(i) == gomma_delete_id.at(i))
                this->data->datatouch->removeat(i);
        }
    }

    gomma_delete_id.clear();

    this->isloading = true;
    update();
}


static inline bool isin(int x, int y, int lastPointx, int lastPointy, int idtratto){
    if(lastPointx - GOMMASIZE < x
            && lastPointx + GOMMASIZE > x
            && lastPointy - GOMMASIZE < y
            && lastPointy + GOMMASIZE > y
            && idtratto != IDORIZZONALE)
            return true;
    return false;
}
