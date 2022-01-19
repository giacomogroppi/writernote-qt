#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"
#include "semaphore.h"
#include "touch/multi_thread_data.h"
#include "core/wmultiplemutex.h"

static QVector<int>         *__data_find;

static page                 *__page;
static const QPointF        *__touch;
static int                  __m_size_gomma;
static datastruct           *__datastruct;

static pthread_mutex_t single_mutex;
static int new_id = 0;

static WMultipleMutex *multi_mutex;

#define REMOVE_STROKE_THREAD_SAVE(counterStroke) \
    do{                                          \
        pthread_mutex_lock(&mutex_write);        \
        __data_find->append(counterStroke);      \
        pthread_mutex_unlock(&mutex_write);      \
    }while(0);


rubber_ui::rubber_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rubber_ui)
{
    ui->setupUi(this);

    this->load_settings();

    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

    this->thread = get_thread_max();
    this->threadData = get_data_max();

    this->countThread = get_thread_used();

    multi_mutex = new WMultipleMutex(countThread - 1, 0);
    pthread_mutex_init(&single_mutex, NULL);
}

rubber_ui::~rubber_ui()
{
    this->save_settings();

    delete ui;
    delete multi_mutex;
}

static inline bool isin(
                    int             size_rubber,
                    const point_s   &__point,
                    const QPointF   &touch)
{
    Q_ASSERT(size_rubber >= 0.0);
    bool isin;

    isin =     (touch.x() - size_rubber) < __point.m_x &&  (touch.x() + size_rubber) > __point.m_x
           &&  (touch.y() - size_rubber) < __point.m_y &&  (touch.y() + size_rubber) > __point.m_y;


    return isin;
}

bool rubber_ui::event(QEvent *event)
{
    if(unlikely(event->type() == QEvent::WindowDeactivate))
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

void rubber_ui::endRubber(datastruct *data)
{
    int i, len = data_to_remove.length();

    Q_ASSERT(data);

    if(m_type_gomma == e_type_rubber::total){

        for(i = 0; i < len; i ++){
            QVector<int> &arr = this->data_to_remove.operator[](i);
            page &page = data->at_mod(i + base);

            order(arr);

            const auto rect = data->get_size_area(arr, i);
            page.removeAndDraw(-1, arr, rect);
        }

        data_to_remove.clear();
    }
}

static bool ifNotInside(stroke &stroke, const double m_size_gomma, const QPointF &pointTouch)
{
    const QRectF &pos = stroke.getBiggerPointInStroke();
    const QPointF &topLeft = pos.topLeft() - QPointF(m_size_gomma, m_size_gomma);
    const QPointF &bottomRigth = pos.bottomRight() + QPointF(m_size_gomma, m_size_gomma);

    W_ASSERT(m_size_gomma >= 0.0);

    //qDebug() << "rubber_ui::actionRubber" << "touch" << lastPoint << "topLeft" << topLeft << "bottomRigth" << bottomRigth;

    /* if the touch point is not within the meaning of the rectangle formed
     * by the top left point and the bottom right point,
     *  we can directly continue with the next stroke. */
    return !datastruct::isinside(topLeft, bottomRigth, pointTouch);
}

void *actionRubberSinglePartial(void *_data)
{
    DataPrivateMuThread *data = (DataPrivateMuThread *) _data;

    QVector<int> stroke_to_remove;

    QVector<int> stroke_mod;
    QVector<int> point_remove;

    int from, to;
    int new_id_priv;
    int lenPoint, counterPoint;

    from = data->from;
    to = data->to;

    stroke_to_remove.reserve(12);
    stroke_mod.reserve(32);
    point_remove.reserve(32);

    W_ASSERT(from <= to);

    for(; from < to; from ++){
        stroke & stroke = __page->atStrokeMod(from);

        if(ifNotInside(stroke, __m_size_gomma, *__touch))
            continue;

        lenPoint = stroke.length();
        for(counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
            const point_s &point = stroke.at(counterPoint);

            if(likely(!isin(__m_size_gomma, point, *__touch)))
                continue;

            if(unlikely(counterPoint < 3)){
                stroke.removeAt(0, counterPoint);
                lenPoint = stroke.length();

                if(stroke.length() < 3)
                    stroke_to_remove.append(data->from);

                continue;
            }

            if(unlikely(counterPoint + 3 > lenPoint)){
                stroke.removeAt(counterPoint, lenPoint - 1);

                if(stroke.length() < 3)
                    stroke_to_remove.append(data->from);

                break;
            }

            W_ASSERT(lenPoint > 6);

            point_remove.removeAt(counterPoint);
            stroke_mod.append(from);

            break;

        }
    }

    W_ASSERT(point_remove.length() == stroke_mod.length());

    from = stroke_mod.length();

    // we don't need to do this operation
    // in order to the list
    pthread_mutex_lock(&single_mutex);

    new_id_priv = new_id;
    new_id += from;

    pthread_mutex_unlock(&single_mutex);

    for(from --; from >= 0; from --){
        cint indexStroke    = stroke_mod.at(from);
        cint indexPoint     = point_remove.at(from);
        __datastruct->changeIdThreadSave(indexPoint, __page->atStrokeMod(indexStroke), *__page, new_id_priv);
        new_id_priv ++;
    }

    multi_mutex->lock(data->id);

    const auto area = __page->get_size_area(point_remove.constData(), point_remove.length());
    __page->removeAndDraw(-1, point_remove, area);

    multi_mutex->unlock(data->id);
    multi_mutex->unlock(data->id - 1);

    return NULL;
}

void *actionRubberSingleTotal(void *_data)
{
    DataPrivateMuThread *data = (DataPrivateMuThread *) _data;
    QVector<int> index_selected;
    int i;
    cint *data_already_find = __data_find->constData();
    cint data_already_len   = __data_find->length();

    index_selected.reserve(16 * 2);

    Q_ASSERT(data->from <= data->to);

#ifdef DEBUGINFO
    qDebug() << data->from << data->to;
#endif

    for(; data->from < data->to; data->from++){
        stroke &stroke = __page->atStrokeMod(data->from);
        int lenPoint = stroke.length();

        if(ifNotInside(stroke, __m_size_gomma, *__touch)) continue;

        for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
            const point_s &point = stroke.at(counterPoint);

            if(unlikely(isin(__m_size_gomma, point, *__touch))){
                // possiamo anche non bloccare gli altri thread nell'appendere
                // tanto di sicuro non staranno cercando il nostro stroke in lista
                // e non lo aggiungeranno

                if(is_present_in_list(data_already_find, data_already_len, data->from))
                    continue;

                index_selected.append(data->from);

                break;

            }
        }
    }

    i = index_selected.length();

    if(!i){
        goto release;
    }

    pthread_mutex_lock(&single_mutex);

    __datastruct->decreaseAlfa(index_selected, __page->count - 1);

    __data_find->append(index_selected);

    pthread_mutex_unlock(&single_mutex);

release:
    return NULL;
}

/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
void rubber_ui::actionRubber(datastruct *data, const QPointF &__lastPoint){
    int lenStroke, counterPage, count;
    const int lenPage = data->lengthPage();
    const bool isTotal = (this->m_type_gomma == e_type_rubber::total);
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);
    void *(*functionToCall)(void *);
    int flag;

    if(isTotal){
        functionToCall = actionRubberSingleTotal;
        flag = ~DATA_PRIVATE_FLAG_SEM;
    }else{
        functionToCall = actionRubberSinglePartial;
        flag = DATA_PRIVATE_FLAG_SEM;
    }

    //pthread_t thread[RUBB_TH];
    //DataPrivateMuThread threadData[RUBB_TH];

    this->base = data->getFirstPageVisible();

    __m_size_gomma =    this->m_size_gomma;
    __datastruct =      data;
    __touch =           &lastPoint;

    new_id = data->maxId() + 1;

    count = 0;

    for(counterPage = base; counterPage < lenPage; counterPage ++){
        page &page = data->at_mod(counterPage);
        int tmp = 0;
        int create;

        if(!page.isVisible()) break;

        lenStroke = page.lengthStroke();

        __page =  &page;

        if(unlikely(data_to_remove.length() - 1 < count))
            data_to_remove.append(QVector<int>());

        __data_find = (QVector<int> *)&data_to_remove.at(count);

        create = DataPrivateMuThreadInit(threadData, NULL, countThread, lenStroke, flag);

        if(!isTotal){
            multi_mutex->blockAll();
            multi_mutex->unlock(create - 1);
        }

        for(tmp = 0; tmp < create; tmp ++){
            pthread_create(&thread[tmp], NULL, functionToCall, &threadData[tmp]);
        }

        for(tmp = 0; tmp < create; tmp ++){
            pthread_join(thread[tmp], NULL);
        }

        if(!isTotal){

            page.mergeList();
        }

        count ++;
    }
}


