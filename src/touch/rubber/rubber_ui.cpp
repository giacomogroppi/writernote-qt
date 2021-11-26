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
    this->save_settings();
    delete ui;
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
QList<int> *rubber_ui::actionRubber(datastruct *data, const QPointF &lastPoint){
    int id;
    int counterStroke, lenStroke, counterPage;
    const int lenPage = data->lengthPage();

    Page.clear();

    if(data->isempty())
        return &Page;

    for(counterPage = 0; counterPage < lenPage && !data->at(counterPage).isVisible(); counterPage ++);


    if(this->m_type_gomma == e_type_rubber::total){
        for(; counterPage < lenPage; counterPage ++){
            const page &page = data->at(counterPage);

            if(!page.isVisible()) break;

            lenStroke = page.lengthStroke();

            for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
                const stroke &stroke = page.atStroke(counterStroke);
                const int lenPoint = stroke.length();
                const int id = stroke.getId();

                for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
                    const point_s &point = data->at_draw(counterPoint, counterPage, counterStroke);
                    if(isin(&point, lastPoint, data) && gomma_delete_id.indexOf(id) == -1){

                        if(Page.indexOf(counterPage) == -1)
                            Page.append(counterPage);

                        gomma_delete_id.append(id);

                        data->decreaseAlfa(id, DECREASE, lenPage);

                        break;
                    }
                }
            }
        }
    }
    else if(this->m_type_gomma == e_type_rubber::partial){
        for(; counterPage < lenPage  && data->at(counterPage).isVisible(); counterPage ++){
            const int lenStroke = data->at(counterPage).lengthStroke();

            for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
                const stroke &stroke = data->at(counterPage).atStroke(counterStroke);
                int counterPoint = 0;
                const int lenPoint = stroke.length();

                for(; counterPoint < lenPoint; counterPoint ++){
                    const point_s &point = data->at_draw(counterPoint, counterPage, counterStroke);
                    if(isin(&point, lastPoint, data)){

                        if(Page.indexOf(counterPage) == -1)
                            Page.append(counterPage);

                        if(data->needtochangeid(counterPoint, counterStroke, counterPage)){
                            data->changeId(counterStroke, counterPage, lenPage);
                        }

                        data->removeat(i, counterPage);
                        --len;
                        --i;
                    }
                }

                __point = &data->at_draw(i, counterPage);


            }
        }

    }

    return &Page;
}

bool rubber_ui::clearList(datastruct *data)
{
    int counterPage, i, lenPage, len;
    const page *page;
    const point_s *point;

    Page.clear();

    if(gomma_delete_id.isEmpty())
        return false;

    for(counterPage = 0, lenPage = data->lengthPage(); counterPage < lenPage; counterPage  ++){
        page = data->at(counterPage);
        len = page->length();

        i = 0;
        page->moveToUserPoint(i);

        for(; i < len; i++){
            point = page->at(i);
            if(gomma_delete_id.indexOf(point->idtratto) != -1){
                if(Page.indexOf(point->page) == -1){
                    if(point->page != counterPage)
                        LOG("rubber_ui::clearList point->page != counterPage", log_ui::possible_bug);
                    Page.append(point->page);
                }
            }
        }
    }

    data->removePointId(gomma_delete_id);

    gomma_delete_id.clear();
    data->triggerNewView(Page, -1, true);
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
