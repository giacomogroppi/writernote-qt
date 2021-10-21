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
bool rubber_ui::actionRubber(datastruct *data, const QPointF &lastPoint){
    if(data->isempty())
        return false;

    int id;
    bool need_reload = false;
    uint i, len, counterPage;
    const uint lenPage = data->lengthPage();
    const point_s *__point;

    this->penna.setStyle(Qt::SolidLine);

    if(this->m_type_gomma == e_type_rubber::total){
        for(counterPage = 0; counterPage < lenPage; counterPage ++){
            len = data->at(counterPage)->length();
            for(i=0; i<len; i++){
                __point = &data->at_draw(i, counterPage);

                if(isin(__point,
                            lastPoint,
                        data) &&
                        gomma_delete_id.indexOf(id) == -1){

                    id = __point->idtratto;

                    need_reload = true;

                    gomma_delete_id.append(id);

                        i = data->decreaseAlfa(id, DECREASE, len);

                    --i;
                }
            }
        }
    }
    else if(this->m_type_gomma == e_type_rubber::partial){
        for(counterPage = 0; counterPage < lenPage; counterPage ++){
            len = data->at(counterPage)->length();
            for(i=0; i<len; i++){
                __point = &data->at_draw(i, counterPage);

                if(isin(__point,
                        lastPoint,
                        data)){
                    need_reload = true;


                    if(data->needtochangeid(i, counterPage)){
                        data->changeId(i, counterPage, lenPage);
                    }

                    data->removeat(i, counterPage);
                    --len;
                    --i;
                }
            }
        }

    }

    return need_reload;
}

bool rubber_ui::clearList(datastruct *data)
{
    if(gomma_delete_id.isEmpty())
        return false;

    data->removePointId(gomma_delete_id);

    gomma_delete_id.clear();

    return true;
}

void rubber_ui::drawAreaRubber(QPainter &painter, const QPointF &point)
{
    return;
    QPen m_penna;
    m_penna.setColor(Qt::white);
    m_penna.setWidth(this->m_size_gomma);

    painter.setPen(m_penna);

    painter.drawPoint(point);
}


bool rubber_ui::isin(const point_s * __point,
                 const QPointF & point_t,
                 const datastruct *data){
    return (point_t.x() - m_size_gomma) < __point->m_x
            && (point_t.x() + m_size_gomma) > __point->m_x
            && (point_t.y() - m_size_gomma) < __point->m_y
            && (point_t.y() + m_size_gomma) > __point->m_y
            && data->isIdUser(__point);
}
