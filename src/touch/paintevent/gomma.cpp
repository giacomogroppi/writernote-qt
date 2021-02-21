#include "../tabletcanvas.h"
#include <QRect>
#include <QPainter>
#include "../method/methoddefinition.h"

static bool isin(double x, double y, double, double , int, rubber_ui *);
static void changeId(int, currenttitle_class *);

#define POSITION_ALFA 3
#define DECREASE 2

void TabletCanvas::gomma(QPainter &painter){
    int i, len, id;
    len = data->datatouch->y.length();

    double x, y;

    x = lastPoint.pos.x();
    y = lastPoint.pos.y();


    if(m_rubber->m_type_gomma == TOTALE){
        for(i=0; i<len; i++){
            if(isin(data->datatouch->x.at(i),
                    data->datatouch->y.at(i),
                    x,
                    y,
                    data->datatouch->idtratto[i],
                    m_rubber)){

                id = data->datatouch->idtratto[i];

                gomma_delete_id.append(id);

                for(; i<len; i++){
                    if(data->datatouch->idtratto[i] != id)
                        break;

                    /* decreases alfa */
                    data->datatouch->color[i].colore[POSITION_ALFA] /= DECREASE;

                }
                i--;
            }
        }
    }
    else if(m_rubber->m_type_gomma == PARZIALE){
        QPen pennatemp;
        pennatemp.setColor(Qt::white);
        pennatemp.setWidth(m_rubber->m_size_gomma);
        for(i=0; i<len; i++){
            if(isin(data->datatouch->x[i],
                    data->datatouch->y[i],
                    x,
                    y,
                    data->datatouch->idtratto[i],
                    m_rubber)){

                painter.drawPoint(data->datatouch->x.at(i), data->datatouch->y.at(i));

                if(data->datatouch->needtochangeid(i)){
                    changeId(i, data);
                }

                data->datatouch->removeat(i);
            }
        }
    }

}

/*
 * the function is call when the user left
 * the screen with the pen
*/
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

#define DELTA 1

static void changeId(int i,
                     currenttitle_class *data){
    int id = data->datatouch->idtratto.last() + DELTA;
    int temp = data->datatouch->idtratto.at(i);

    for(int k=0; data->datatouch->idtratto.at(k) == temp; k++)
        data->datatouch->idtratto[k] = id;
}

static bool isin(double x,
                 double y,
                 double lastPointx,
                 double lastPointy,
                 int idtratto,
                 rubber_ui *m_rubber){
    if(lastPointx - m_rubber->m_size_gomma < x
            && lastPointx + m_rubber->m_size_gomma > x
            && lastPointy - m_rubber->m_size_gomma < y
            && lastPointy + m_rubber->m_size_gomma > y
            && idtratto != IDORIZZONALE
            && idtratto != IDVERTICALE)
            return true;
    return false;
}
