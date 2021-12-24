#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"

#define RUBB_TH 20
struct RuDataPrivate{
    int from, to;
    QList<stroke> *__data;
    const QPointF *touch;
};

#define CTRL_LEN_STROKE(stroke, pageStroke, indexStroke, lenStroke) \
    if(stroke.length() < 3){ \
        pageStroke.removeAt(counterStroke); \
        lenStroke = page.lengthStroke(); \
        break; \
    }

rubber_ui::rubber_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rubber_ui)
{
    ui->setupUi(this);

    this->load_settings();

    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

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

static bool ifNotInside(stroke &stroke, const double m_size_gomma, const QPointF &pointTouch)
{
    const QRectF &pos = stroke.getBiggerPointInStroke();
    const QPointF &topLeft = pos.topLeft() - QPointF(m_size_gomma, m_size_gomma);
    const QPointF &bottomRigth = pos.bottomRight() + QPointF(m_size_gomma, m_size_gomma);

    Q_ASSERT(m_size_gomma >= 0.0);

    //qDebug() << "rubber_ui::actionRubber" << "touch" << lastPoint << "topLeft" << topLeft << "bottomRigth" << bottomRigth;

    /* if the touch point is not within the meaning of the rectangle formed
     * by the top left point and the bottom right point,
     *  we can directly continue with the next stroke. */
    return !datastruct::isinside(topLeft, bottomRigth, pointTouch);
}

/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
const QList<int> &rubber_ui::actionRubber(datastruct *data, const QPointF &__lastPoint){
    int counterStroke, lenStroke, counterPage;
    const int lenPage = data->lengthPage();
    const bool isTotal = this->m_type_gomma == e_type_rubber::total;
    char mod;
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);

    pthread_t thread[RUBB_TH];
    RuDataPrivate threadData[RUBB_TH];


    Page.clear();

    counterPage = data->getFirstPageVisible();

    for(mod = 0; counterPage < lenPage; counterPage ++){
        page &page = data->at_mod(counterPage);

        if(!page.isVisible()) break;

        lenStroke = page.lengthStroke();



    }

    for(mod = 0; counterPage < lenPage; counterPage ++){
        page &page = data->at_mod(counterPage);

        if(!page.isVisible()) break;

        lenStroke = page.lengthStroke();

        for(counterStroke = 0; counterStroke < lenStroke; counterStroke++){
            stroke &stroke = page.atStrokeMod(counterStroke);
            int lenPoint = stroke.length();
            const int id = stroke.getId();

            if(ifNotInside(stroke, m_size_gomma, lastPoint)) continue;

            for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
                const point_s &point = stroke.at(counterPoint);

                if(isin(point, lastPoint)){
                    mod = 1;
                    
                    if(isTotal){
                        if(IS_PRESENT_IN_LIST(gomma_delete_id, id)) continue;

                        gomma_delete_id.append(id);

                        data->decreaseAlfa(stroke, page, DECREASE);

                        break;
                    }
                    else{
                        if(counterPoint < 3){
                            stroke.removeAt(0, counterPoint);
                            lenPoint = stroke.length();

                            CTRL_LEN_STROKE(stroke, page, counterStroke, lenStroke);

                            continue;
                        }

                        if(counterPoint + 3 > lenPoint){
                            stroke.removeAt(counterPoint, lenPoint - 1);

                            CTRL_LEN_STROKE(stroke, page, counterStroke, lenStroke);

                            break;
                        }

                        Q_ASSERT(lenPoint > 6);
                        stroke.removeAt(counterPoint);
                        data->changeId(counterPoint, counterStroke, counterPage);

                        lenStroke = page.lengthStroke();

                        break;
                    }
                }
            }

            if(mod){
                append_if_not_present_order(Page, counterPage);
                //IF_NOT_PRESENT_APPEND(Page, counterPage);
                mod = 0;
            }
        }
    }

    return Page;
}

bool rubber_ui::clearList(datastruct *data)
{
    data->removeAndTrigger(gomma_delete_id);
    //data->removePointId(gomma_delete_id, &Page);

    gomma_delete_id.clear();
    //data->triggerNewView(Page, -1, true);
    return Page.length();
}

inline bool rubber_ui::isin(
                    const point_s   &__point,
                    const QPointF   &touch)
{
    Q_ASSERT(m_size_gomma >= 0.0);
    bool isin;

    isin =     (touch.x() - m_size_gomma) < __point.m_x &&  (touch.x() + m_size_gomma) > __point.m_x
           &&  (touch.y() - m_size_gomma) < __point.m_y &&  (touch.y() + m_size_gomma) > __point.m_y;


    return isin;
}

bool rubber_ui::isin(
        const QPointF &point,
        const QPointF &point_t)
{
    return (point_t.x() - m_size_gomma) < point.x()
            && (point_t.x() + m_size_gomma) > point.x()
            && (point_t.y() - m_size_gomma) < point.y()
            && (point_t.y() + m_size_gomma) > point.y();
}

void *actionRubberSingle(void *data)
{

}
