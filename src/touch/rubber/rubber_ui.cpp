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
const QList<int> &rubber_ui::actionRubber(datastruct *data, const QPointF &__lastPoint){
    int counterStroke, lenStroke, counterPage;
    const int lenPage = data->lengthPage();
    const QPointF &translation = data->getPointFirstPage();

    const QPointF &lastPoint = __lastPoint / data->getZoom() - data->getPointFirstPage();

    Page.clear();

    if(data->isempty()) return Page;

    for(counterPage = 0; counterPage < lenPage && !data->at(counterPage).isVisible(); counterPage ++);

    //qDebug() << lastPoint << __lastPoint;

    if(this->m_type_gomma == e_type_rubber::total){
        for(; counterPage < lenPage; counterPage ++){
            page &page = data->at_mod(counterPage);

            if(!page.isVisible()) break;

            lenStroke = page.lengthStroke();

            for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
                stroke &stroke = page.atStrokeMod(counterStroke);
                const int lenPoint = stroke.length();
                const int id = stroke.getId();

                {
                    const QRectF &pos = stroke.getBiggerPointInStroke();
                    const QPointF &topLeft = pos.topLeft() + translation - QPointF(m_size_gomma, m_size_gomma);
                    const QPointF &bottomRigth = pos.bottomRight() + translation + QPointF(m_size_gomma, m_size_gomma);

                    //qDebug() << "pos" << pos << "topLeft" << topLeft << "bottomRigth" << bottomRigth;

                    /* if the touch point is not within the meaning of the rectangle formed
                     * by the top left point and the bottom right point,
                     *  we can directly continue with the next stroke. */
                    if(!datastruct::isinside(topLeft, bottomRigth, lastPoint)){
                        //qDebug() << "rubber_ui::actionRubber continue";
                        continue;
                    }
                }

                for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
                    const point_s &point = stroke.at(counterPoint);

                    if(isin(point, lastPoint) && gomma_delete_id.indexOf(id) == -1){

                        if(Page.indexOf(counterPage) == -1)
                            Page.append(counterPage);

                        gomma_delete_id.append(id);

                        //qDebug() << "rubber_ui::actionRubber color" << stroke.getColor().alpha();

                        stroke.setAlfaColor(stroke.getColor().alpha() / DECREASE);

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
                int lenPoint = stroke.length();

                for(; counterPoint < lenPoint; counterPoint ++){
                    const point_s &point = data->at_draw(counterPoint, counterPage, counterStroke);
                    if(isin(point, lastPoint)){

                        if(Page.indexOf(counterPage) == -1)
                            Page.append(counterPage);

                        if(data->needtochangeid(counterPoint, counterStroke, counterPage)){
                            data->changeId(counterStroke, counterPage, lenPage);
                        }

                        data->at_mod(counterPage).atStrokeMod(counterStroke).removeAt(counterPoint);

                        --lenPoint;
                        --counterPoint;
                    }
                }

            }
        }

    }

    //qDebug() << "QList<int> *rubber_ui::actionRubber" << Page << this->gomma_delete_id;

    return Page;
}

bool rubber_ui::clearList(datastruct *data)
{
    int counterPage, counterStroke, lenPage, lenStroke;
    const page *page;

    Page.clear();

    if(gomma_delete_id.isEmpty())
        return false;

    for(counterPage = 0, lenPage = data->lengthPage(); counterPage < lenPage; counterPage  ++){
        page = &data->at(counterPage);
        lenStroke = page->lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            const stroke &stroke = page->atStroke(counterStroke);
            if(gomma_delete_id.indexOf(stroke.getId()) != -1){
                if(Page.indexOf(stroke.getPage()) == -1){

                    if(stroke.getPage() != counterPage)
                        LOG("rubber_ui::clearList point->page != counterPage", log_ui::possible_bug);

                    Page.append(stroke.getPage());
                }
            }
        }
    }

    data->removePointId(gomma_delete_id);

    gomma_delete_id.clear();
    data->triggerNewView(Page, -1, true);
    return true;
}

bool rubber_ui::isin(const point_s & __point,
                 const QPointF & point_t){
    return (point_t.x() - m_size_gomma) < __point.m_x
            && (point_t.x() + m_size_gomma) > __point.m_x
            && (point_t.y() - m_size_gomma) < __point.m_y
            && (point_t.y() + m_size_gomma) > __point.m_y;
}

bool rubber_ui::isin(const QPointF &point, const QPointF &point_t)
{
    return (point_t.x() - m_size_gomma) < point.x()
            && (point_t.x() + m_size_gomma) > point.x()
            && (point_t.y() - m_size_gomma) < point.y()
            && (point_t.y() + m_size_gomma) > point.y();
}
