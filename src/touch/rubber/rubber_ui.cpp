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
    // const QPointF &translation = data->getPointFirstPage();
    char mod;
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);

    Page.clear();

    for(counterPage = 0; counterPage < lenPage && !data->at(counterPage).isVisible(); counterPage ++);

    //qDebug() << lastPoint << __lastPoint;

    for(mod = 0; counterPage < lenPage; counterPage ++){
        page &page = data->at_mod(counterPage);

        if(!page.isVisible()) break;

        lenStroke = page.lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            stroke &stroke = page.atStrokeMod(counterStroke);
            int lenPoint = stroke.length();
            const int id = stroke.getId();

            {
                const QRectF &pos = stroke.getBiggerPointInStroke();
                const QPointF &topLeft = pos.topLeft() - QPointF(m_size_gomma, m_size_gomma);
                const QPointF &bottomRigth = pos.bottomRight() + QPointF(m_size_gomma, m_size_gomma);

                Q_ASSERT(m_size_gomma >= 0.0);

                qDebug() << "rubber_ui::actionRubber" << "touch" << lastPoint << "topLeft" << topLeft << "bottomRigth" << bottomRigth;

                /* if the touch point is not within the meaning of the rectangle formed
                 * by the top left point and the bottom right point,
                 *  we can directly continue with the next stroke. */
                if(!datastruct::isinside(topLeft, bottomRigth, lastPoint)){
                    qDebug() << "rubber_ui::actionRubber" << "continue";
                    continue;
                }
            }

            for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
                const point_s &point = stroke.at(counterPoint);

                if(isin(point, lastPoint)){
                    if(this->m_type_gomma == e_type_rubber::total && gomma_delete_id.indexOf(id) == -1){
                        mod = 1;

                        gomma_delete_id.append(id);

                        stroke.setAlfaColor(stroke.getColor().alpha() / DECREASE);

                        break;
                    }
                    else if(this->m_type_gomma == e_type_rubber::partial){
                        if(data->needtochangeid(counterPoint, counterStroke, counterPage)){
                            data->changeId(counterPoint, counterStroke, counterPage);
                        }
                        mod = 1;
                        data->at_mod(counterPage).atStrokeMod(counterStroke).removeAt(counterPoint);

                        --lenPoint;
                        --counterPoint;
                    }
                }
            }
            if(mod){
                if(Page.indexOf(counterPage) == -1)
                    Page.append(counterPage);
                mod = 0;
            }
        }
    }

    return Page;
}

bool rubber_ui::clearList(datastruct *data)
{
    if(gomma_delete_id.isEmpty())
        return false;

    data->removePointId(gomma_delete_id, &Page);

    gomma_delete_id.clear();
    data->triggerNewView(Page, -1, true);
    return true;
}

inline bool rubber_ui::isin(const point_s & __point,
                 const QPointF & touch){
    Q_ASSERT(m_size_gomma >= 0.0);
    bool isin;

    isin =     (touch.x() - m_size_gomma) < __point.m_x &&  (touch.x() + m_size_gomma) > __point.m_x
           &&  (touch.y() - m_size_gomma) < __point.m_y &&  (touch.y() + m_size_gomma) > __point.m_y;

    //qDebug() << "rubber_ui::isin" << isin;

    return isin;
}

bool rubber_ui::isin(const QPointF &point, const QPointF &point_t)
{
    return (point_t.x() - m_size_gomma) < point.x()
            && (point_t.x() + m_size_gomma) > point.x()
            && (point_t.y() - m_size_gomma) < point.y()
            && (point_t.y() + m_size_gomma) > point.y();
}
