#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"
#include "touch/multi_thread_data.h"

void * actionRubberSingle (void *);

#define RUBB_TH 8
#ifdef DEBUG_THREAD
# undef RUBB_TH
# define RUBB_TH DEBUG_THREAD
#endif

static QVector<int>         *__data_find;

static page                 *__page;
static const QPointF        *__touch;
static int                  __m_size_gomma;
static bool                 __isTotal;
static datastruct           *__datastruct;

static int new_id = 0;

static pthread_mutex_t mutex_write;

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

    pthread_mutex_init(&mutex_write, NULL);
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
            page.removeAndDraw(-1, (int *)arr.constData(), arr.length(), rect);
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

/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
void rubber_ui::actionRubber(datastruct *data, const QPointF &__lastPoint){
    int lenStroke, counterPage, count;
    const int lenPage = data->lengthPage();
    const bool isTotal = (this->m_type_gomma == e_type_rubber::total);
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);

    pthread_t thread[RUBB_TH];
    DataPrivateMuThread threadData[RUBB_TH];

    this->base = data->getFirstPageVisible();

    __m_size_gomma =    this->m_size_gomma;
    __isTotal =         isTotal;
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

        if(data_to_remove.length() - 1 < count)
            data_to_remove.append(QVector<int>());

        __data_find = (QVector<int> *)&data_to_remove.at(count);

        create = DataPrivateMuThreadInit(threadData, RUBB_TH, lenStroke);

        for(tmp = 0; tmp < create; tmp ++){
            pthread_create(&thread[tmp], NULL, actionRubberSingle, &threadData[tmp]);
        }

        for(tmp = 0; tmp < create; tmp ++){
            pthread_join(thread[tmp], NULL);
        }

        if(isTotal){

        }
        else{
            for(int tmp = __data_find->length() - 1; tmp >= 0; tmp --){
                page.removeAt(__data_find->at(tmp));
            }
            page.mergeList();
        }

        count ++;
    }
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

void *actionRubberSingle(void *_data)
{
    DataPrivateMuThread *data = (DataPrivateMuThread *) _data;

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

            if(isin(__m_size_gomma, point, *__touch)){
                if(__isTotal){
                    // possiamo anche non bloccare gli altri thread nell'appendere
                    // tanto di sicuro non staranno cercando il nostro stroke

                    if(is_present_in_list(__data_find->constData(), __data_find->length(), data->from))
                        continue;

                    pthread_mutex_lock(&mutex_write);

                    __data_find->append(data->from);

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

                    __data_find->append(data->from);

                    pthread_mutex_unlock(&mutex_write);

                    break;
                }
            }
        }
    }

    return NULL;
}
