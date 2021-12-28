#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"

void * actionRubberSingle (void *);

#define RUBB_TH 8
// we store the index of stroke to remove
static int              *__res_index;
static int              *__len;
static int              *__per;

static page             *__page;
static const QPointF    *__touch;
static int              __m_size_gomma;
static bool             __isTotal;
static datastruct       *__datastruct;

static int new_id = 0;

static pthread_mutex_t mutex_write;

struct RuDataPrivate{
    int from, to;
};

#define REMOVE_STROKE_THREAD_SAVE(counterStroke) \
    pthread_mutex_lock(&mutex_write);       \
    __res_index[*__len] = counterStroke;    \
    (*__len) ++;                            \
    pthread_mutex_unlock(&mutex_write);


rubber_ui::rubber_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rubber_ui)
{
    ui->setupUi(this);

    this->load_settings();

    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

    gomma_delete_id = (int *) malloc( sizeof(int) * RU_INDEX_LEN);
    len_index = 0;

    pthread_mutex_init(&mutex_write, NULL);

    __per =     &this->per;
    __len =     &this->len_index;
    __res_index = this->gomma_delete_id;
}

rubber_ui::~rubber_ui()
{
    this->save_settings();

    free(gomma_delete_id);

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
void rubber_ui::actionRubber(datastruct *data, const QPointF &__lastPoint){
    int lenStroke, counterPage;
    const int lenPage = data->lengthPage();
    const bool isTotal = (this->m_type_gomma == e_type_rubber::total);
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);

    pthread_t thread[RUBB_TH];
    RuDataPrivate threadData[RUBB_TH];

    Page.clear();

    counterPage = data->getFirstPageVisible();

    __m_size_gomma =    this->m_size_gomma;
    __isTotal =         isTotal;
    __datastruct =      data;
    __touch =           &lastPoint;

    new_id = data->maxId() + 1;

    for(; counterPage < lenPage; counterPage ++){
        page &page = data->at_mod(counterPage);
        int tmp = 0;
        int done, div;

        if(!page.isVisible()) break;

        lenStroke = page.lengthStroke();
        done = 0;

        div = div_ecc(RUBB_TH, lenStroke);

        len_index = 0;
        __page =  &page;

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


        for(int i = 0; i < this->len_index; i++){
            page.removeAt(this->gomma_delete_id[i]);
        }

        page.mergeList();
    }
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

/*static inline bool isin(
        int size_rubber,
        const QPointF &point,
        const QPointF &point_t)
{
    return (point_t.x() - size_rubber) < point.x()
            && (point_t.x() + size_rubber) > point.x()
            && (point_t.y() - size_rubber) < point.y()
            && (point_t.y() + size_rubber) > point.y();
}*/

void *actionRubberSingle(void *_data)
{
    RuDataPrivate *data = (RuDataPrivate *) _data;

    Q_ASSERT(data->from < data->to);

    for(; data->from < data->to; data->from++){
        stroke &stroke = __page->atStrokeMod(data->from);
        int lenPoint = stroke.length();

        if(ifNotInside(stroke, __m_size_gomma, *__touch)) continue;

        for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
            const point_s &point = stroke.at(counterPoint);

            if(isin(__m_size_gomma, point, *__touch)){
                if(__isTotal){
                    pthread_mutex_lock(&mutex_write);

                    __res_index[*__len] = data->from;
                    (*__len) ++;
                    if(*__len > RU_INDEX_LEN * (*__per)){
                        (*__per) ++;
                        __res_index = (int *)realloc(__res_index, sizeof(int) * (*__per));

                        qDebug() << "Need to realloc";
                    }

                    __datastruct->decreaseAlfa(stroke, *__page, DECREASE);

                    pthread_mutex_unlock(&mutex_write);

                    break;
                }
                else{
                    if(counterPoint < 3){
                        stroke.removeAt(0, counterPoint);
                        lenPoint = stroke.length();

                        if(stroke.length() < 3)
                            REMOVE_STROKE_THREAD_SAVE(data->from);

                        continue;
                    }

                    if(counterPoint + 3 > lenPoint){
                        stroke.removeAt(counterPoint, lenPoint - 1);

                        if(stroke.length() <  3)
                            REMOVE_STROKE_THREAD_SAVE(data->from);

                        break;
                    }

                    Q_ASSERT(lenPoint > 6);

                    stroke.removeAt(counterPoint);

                    pthread_mutex_lock(&mutex_write);
                    __datastruct->changeId(counterPoint, stroke, *__page, new_id);

                    new_id ++;

                    __res_index[*__len] = data->from;
                    (*__len) ++;

                    pthread_mutex_unlock(&mutex_write);

                    break;
                }
            }
        }
    }

    return NULL;
}
