#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"
#include "core/listthreadsave.h"

void * actionRubberSingle (void *);

#define RUBB_TH 20
// we store the index of stroke to remove
static ListThreadSave<int>  *__res_index;
static QList<stroke>        *__data;
static const QPointF        *__touch;
static int                  __m_size_gomma;
static bool                 __isTotal;
static datastruct           __datastruct;

struct RuDataPrivate{
    int from, to;
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

    __res_index =   new ListThreadSave<int>;
    __data =        new QList<stroke>;
}

rubber_ui::~rubber_ui()
{
    this->save_settings();

    delete __res_index;
    delete __data;

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

    __res_index->from(gomma_delete_id);
    __m_size_gomma =    this->m_size_gomma;
    __isTotal =         isTotal;
    __datastruct =      data;

    for(mod = 0; counterPage < lenPage; counterPage ++){
        page &page = data->at_mod(counterPage);
        int tmp = 0;
        int done, div;

        if(!page.isVisible()) break;

        lenStroke = page.lengthStroke();
        done = 0;

        div = div_ecc(RUBB_TH, lenStroke);

        __data =  &page.m_stroke;
        __touch = &lastPoint;

        for(tmp = 0; tmp < RUBB_TH && tmp < lenStroke; tmp ++){
            threadData[tmp].from = done;
            threadData[tmp].to = done + div;
            done += div;
        }

        threadData[RUBB_TH - 1].to = lenStroke;

        for(tmp = 0; tmp < RUBB_TH && tmp < lenStroke; tmp ++){
            pthread_create(&thread[tmp], NULL, actionRubberSingle, &threadData[tmp]);
        }

        for(tmp = 0; tmp < RUBB_TH && tmp < lenStroke; tmp ++){
            pthread_join(thread[tmp], NULL);
        }

        for(tmp = 0; tmp < RUBB_TH && tmp < lenStroke; tmp ++){
            if(threadData[tmp].to == -1){
                Page.append(counterPage);
                break;
            }
        }

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

static inline bool isin(
                    int size_rubber,
                    const point_s   &__point,
                    const QPointF   &touch)
{
    Q_ASSERT(size_rubber >= 0.0);
    bool isin;

    isin =     (touch.x() - size_rubber) < __point.m_x &&  (touch.x() + size_rubber) > __point.m_x
           &&  (touch.y() - size_rubber) < __point.m_y &&  (touch.y() + size_rubber) > __point.m_y;


    return isin;
}

static inline bool isin(
        int size_rubber,
        const QPointF &point,
        const QPointF &point_t)
{
    return (point_t.x() - size_rubber) < point.x()
            && (point_t.x() + size_rubber) > point.x()
            && (point_t.y() - size_rubber) < point.y()
            && (point_t.y() + size_rubber) > point.y();
}

void *actionRubberSingle(void *_data)
{
    RuDataPrivate *data = (RuDataPrivate *) _data;
    char mod;

    Q_ASSERT(data->to < data->from);

    for(; data->to < data->from; data->to++){
        stroke &stroke = __data->operator[](data->to);
        int lenPoint = stroke.length();
        const int id = stroke.getId();
        mod = 0;

        if(ifNotInside(stroke, __m_size_gomma, *__touch)) continue;

        for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
            const point_s &point = stroke.at(counterPoint);

            if(isin(__m_size_gomma, point, *__touch)){
                mod = 1;

                if(__isTotal){
                    __res_index->forceLock();
                    if(IS_PRESENT_IN_LIST(__res_index->m_list, id)) continue;

                    __res_index->m_list.append(id);

                    __res_index->unlock();

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
