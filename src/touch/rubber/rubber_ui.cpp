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
    this->m_type_gomma = TOTALE;
    ui->totale_button->setChecked(this->m_type_gomma == TOTALE);
    ui->partial_button->setChecked(this->m_type_gomma == PARZIALE);
}

void rubber_ui::on_partial_button_clicked()
{
    m_type_gomma = PARZIALE;
    ui->totale_button->setChecked(this->m_type_gomma == TOTALE);
    ui->partial_button->setChecked(this->m_type_gomma == PARZIALE);
}


/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
bool rubber_ui::actionRubber(datastruct *data, QPointF lastPoint, QPainter &painter){
    int id;
    unsigned int i, len;
    len = data->m_point.length();

    double x, y;

    x = lastPoint.x();
    y = lastPoint.y();


    if(this->m_type_gomma == TOTALE){
        for(i=0; i<len; i++){
            if(isin(data->m_point.at(i).m_x,
                    data->m_point.at(i).m_y,
                    x,
                    y,
                    data->m_point.at(i).idtratto)){

                id = data->m_point.at(i).idtratto;

                gomma_delete_id.append(id);
                for(; i<len; i++){
                    if(data->m_point.at(i).idtratto != id)
                        break;

                    data->m_point.operator[](i).m_color.colore[POSITION_ALFA] /= DECREASE;
                }
                i--;
            }
        }

    }
    else if(this->m_type_gomma == PARZIALE){
        QPen pennatemp;
        pennatemp.setColor(Qt::white);
        pennatemp.setWidth(this->m_size_gomma);

        painter.setPen(penna);

        for(i=0; i<len; i++){
            if(isin(data->m_point.at(i).m_x,
                    data->m_point.at(i).m_y,
                    x,
                    y,
                    data->m_point.at(i).idtratto)){

                painter.drawPoint(data->m_point.at(i).m_x, data->m_point.at(i).m_y);

                if(data->needtochangeid(i)){
                    changeId(i, data);
                }

                data->removeat(i);
            }
        }

    }

    return true;
}

/*
 * call when the user left the screen
*/
bool rubber_ui::clearList(datastruct *data)
{
    if(gomma_delete_id.isEmpty())
        return false;

    int k, len_cancella;

    unsigned int i, len;

    len = data->m_point.length();
    len_cancella = gomma_delete_id.length();

    for(k=0; k<len_cancella; k++){
        for(i=0; i<len; i++){
            if(data->m_point.at(i).idtratto == gomma_delete_id.at(i))
                data->removeat(i);
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
            && idtratto != IDVERTICALE)
            return true;
    return false;
}

#define DELTA 1

void rubber_ui::changeId(unsigned int i,
                     datastruct *data){
    int id = data->m_point.last().idtratto + DELTA;
    int temp = data->m_point.at(i).idtratto;

    for(; data->m_point.at(i).idtratto == temp; i++)
        data->m_point.operator[](i).idtratto = id;
}
