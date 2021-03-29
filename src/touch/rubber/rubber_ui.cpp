#include "rubber_ui.h"
#include "ui_rubber_ui.h"

rubber_ui::rubber_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rubber_ui)
{
    ui->setupUi(this);

    this->load_settings();

    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

}


rubber_ui::~rubber_ui()
{
    delete ui;
    this->save_settings();
}

bool rubber_ui::event(QEvent *event){
    if(event->type() == QEvent::WindowDeactivate)
        this->hide();

    return QWidget::event(event);
}

void rubber_ui::on_totale_button_clicked()
{
    this->m_type_gomma = e_type_rubber::total;

    this->update_data();
}

void rubber_ui::on_partial_button_clicked()
{
    m_type_gomma = e_type_rubber::partial;

    this->update_data();
}

/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
bool rubber_ui::actionRubber(datastruct *data, QPointF lastPoint, QPainter &painter){
    if(data->isempty())
        return false;

    int id;
    bool need_reload = false;
    unsigned int i, len;

    len = data->m_point.length();

    double x, y;

    x = lastPoint.x();
    y = lastPoint.y();

    const point_s *__point = & data->m_point.first();

    this->penna.setStyle(Qt::SolidLine);

    if(this->m_type_gomma == e_type_rubber::total){
        for(i=0; i<len; i++){
            __point = &data->m_point.at(i);

            if(isin(__point->m_x,
                    __point->m_y,
                    x,
                    y,
                    __point->idtratto)){

                id = __point->idtratto;

                need_reload = true;

                gomma_delete_id.append(id);

                for(; i<len && data->m_point.at(i).idtratto == id; i++){
                    data->m_point.operator[](i).m_color.colore[POSITION_ALFA] /= DECREASE;
                }

                --i;

                __point = &data->m_point.at(i);
            }
        }
    }
    else if(this->m_type_gomma == e_type_rubber::partial){
        QPen pennatemp;
        pennatemp.setColor(Qt::white);
        pennatemp.setWidth(1);

        painter.setPen(penna);

        for(i=0; i<len; i++){
            if(isin(data->m_point.at(i).m_x,
                    data->m_point.at(i).m_y,
                    x,
                    y,
                    data->m_point.at(i).idtratto)){
                need_reload = true;

                painter.drawPoint(data->m_point.at(i).m_x, data->m_point.at(i).m_y);

                if(data->needtochangeid(i)){
                    data->changeId(i);
                }

                data->removeat(i);
                len --;
            }
        }

    }

    return need_reload;
}

static unsigned int m(QList<int> * list, int number, int index, int len){
    unsigned f;
    len = list->length();

    for(f = 0; index<len; index++){
        if(list->at(index) == number)
            ++f;
    }

    return f;
}

/*
 * call when the user left the screen
 * this function remove the duplicate number
*/
static void removeR(QList<int> *list){
    int i, len, time, k;

    len = list->length();

    for(i=0; i<len; i++){
        time = m(list, list->at(i), i, len);
        if(time){
            for(k=0; k<time; k++){
                list->removeAt(i);
            }
        }

        len -= time;

    }
}

bool rubber_ui::clearList(datastruct *data)
{
    if(gomma_delete_id.isEmpty())
        return false;

    int k, len_cancella;

    removeR(&this->gomma_delete_id);

    unsigned int i, len;

    len = data->m_point.length();
    len_cancella = gomma_delete_id.length();

    for(k=0; k<len_cancella; k++){
        for(i=0; i<len; i++){
            if(data->m_point.at(i).idtratto == gomma_delete_id.at(k)){
                data->removeat(i);
                len --;
            }
        }
    }

    gomma_delete_id.clear();

    return true;
}


bool rubber_ui::isin(double x,
                 double y,
                 double lastPointx,
                 double lastPointy,
                 int idtratto){
    if(lastPointx - m_size_gomma < x
            && lastPointx + m_size_gomma > x
            && lastPointy - m_size_gomma < y
            && lastPointy + m_size_gomma > y
            && idtratto != IDORIZZONALE
            && idtratto != IDVERTICALE
            && idtratto != IDTRATTOZERO)
            return true;
    return false;
}

