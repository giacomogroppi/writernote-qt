#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"
#include "semaphore.h"
#include "touch/multi_thread_data.h"
#include "core/wmultiplemutex.h"

struct RubberPrivateData{
    QVector<int>    *data_find;
    page            *__page;
    const QPointF   *touch;
    datastruct      *data;
    QVector<int>    *data_to_remove;
    QVector<int>    *stroke_mod;
};

static int             __m_size_gomma;
static pthread_mutex_t single_mutex;

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

    pthread_mutex_init(&single_mutex, NULL);
}

rubber_ui::~rubber_ui()
{
    this->save_settings();

    delete ui;
}

static force_inline bool isin(
                    int             size_rubber,
                    const point_s   &__point,
                    const QPointF   &touch)
{
    W_ASSERT(size_rubber >= 0.0);

    if(unlikely( (touch.x() - size_rubber) < __point.m_x &&  (touch.x() + size_rubber) > __point.m_x
            &&   (touch.y() - size_rubber) < __point.m_y &&  (touch.y() + size_rubber) > __point.m_y))
        return true;

    return false;
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

    W_ASSERT(data);
    qDebug() << "rubber_ui::endRubber";

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

void *actionRubberSinglePartial(void *__data)
{
    DataPrivateMuThread *data = (DataPrivateMuThread *) __data;
    RubberPrivateData *private_data = (RubberPrivateData *)data->extra;

    QVector<int> stroke_to_remove;
    QVector<int> stroke_mod;
    QVector<int> point_remove;

    int from, to;
    int lenPoint, counterPoint;

    page *_page             = private_data->__page;
    const QPointF *_touch   = private_data->touch;
    datastruct *_datastruct = private_data->data;

    QVector<int> *_data_to_remove = private_data->data_to_remove;

    from = data->from;
    to = data->to;

    stroke_to_remove.reserve(12);
    stroke_mod.reserve(32);
    point_remove.reserve(32);

    W_ASSERT(from <= to);

    for(; from < to; from ++){
        stroke & stroke = _page->atStrokeMod(from);

        if(ifNotInside(stroke, __m_size_gomma, *_touch))
            continue;

        lenPoint = stroke.length();
        for(counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
            const point_s &point = stroke.at(counterPoint);

            if(likely(!isin(__m_size_gomma, point, *_touch)))
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

            W_ASSERT(lenPoint >= 6);

            point_remove.append (counterPoint);
            stroke_mod.append   (from);

            break;

        }
    }

    from = stroke_mod.length();

    if(!stroke_to_remove.length() && !from)
        goto free;

    // we don't need to do this operation
    // in order to the list

    for(from --; from >= 0; from --){
        cint indexStroke    = stroke_mod.at(from);
        cint indexPoint     = point_remove.at(from);

        _datastruct->changeIdThreadSave(indexPoint, _page->atStrokeMod(indexStroke), *_page);
    }

    qDebug() << stroke_mod;
    pthread_mutex_lock(&single_mutex);
    _data_to_remove->append(stroke_to_remove);
    private_data->stroke_mod->append(stroke_mod);
    pthread_mutex_unlock(&single_mutex);

free:
    return NULL;
}

void *actionRubberSingleTotal(void *__data)
{
    DataPrivateMuThread *data = (DataPrivateMuThread *) __data;
    RubberPrivateData *private_data = (RubberPrivateData *)data->extra;

    QVector<int> index_selected;
    cint data_already_len   = private_data->data_find->length();

    page *_page             = private_data->__page;
    const QPointF *_touch   = private_data->touch;
    QVector<int> *_al_find  = private_data->data_find;

    index_selected.reserve(32);

    Q_ASSERT(data->from <= data->to);

    qDebug() << data->from << data->to << *_al_find;

    for(; data->from < data->to; data->from++){
        stroke &stroke = _page->atStrokeMod(data->from);
        int lenPoint = stroke.length();

        if(is_present_in_list(_al_find->constData(), data_already_len, data->from))
            continue;

        if(ifNotInside(stroke, __m_size_gomma, *_touch)) continue;

        for(int counterPoint = 0; counterPoint < lenPoint; counterPoint ++){
            const point_s &point = stroke.at(counterPoint);

            if(unlikely(isin(__m_size_gomma, point, *_touch))){
                // possiamo anche non bloccare gli altri thread nell'appendere
                // tanto di sicuro non staranno cercando il nostro stroke in lista
                // e non lo aggiungeranno

                index_selected.append(data->from);

                break;

            }
        }
    }

    if(index_selected.isEmpty()){
        goto release;
    }

    pthread_mutex_lock(&single_mutex);

    _al_find->append(index_selected);
    private_data->data->decreaseAlfa(index_selected, _page->count - 1);

    pthread_mutex_unlock(&single_mutex);

release:
    return NULL;
}

/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
void rubber_ui::actionRubber(datastruct *data, const QPointF &__lastPoint)
{
    int lenStroke, counterPage, count;
    const int lenPage = data->lengthPage();
    const bool isTotal = (this->m_type_gomma == e_type_rubber::total);
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);
    void *(*functionToCall)(void *);
    int flag, tmp = 0, create;
    RubberPrivateData dataPrivate;

    QVector<int> stroke_mod;
    stroke_mod.reserve(32);

    if(isTotal){
        functionToCall = actionRubberSingleTotal;
        flag = ~DATA_PRIVATE_FLAG_SEM;
    }else{
        functionToCall = actionRubberSinglePartial;
        flag = DATA_PRIVATE_FLAG_SEM;
    }

    this->base = data->getFirstPageVisible();

    dataPrivate.data    = data;
    dataPrivate.touch   = &lastPoint;
    dataPrivate.stroke_mod      = &stroke_mod;

    __m_size_gomma =    this->m_size_gomma;

    count = 0;

    for(counterPage = base; counterPage < lenPage; counterPage ++){
        dataPrivate.__page = &data->at_mod(counterPage);

        if(!dataPrivate.__page->isVisible()) break;

        lenStroke = dataPrivate.__page->lengthStroke();

        if(unlikely(data_to_remove.length() <= count))
            data_to_remove.append(QVector<int>());

        dataPrivate.data_find       = (QVector<int> *)&data_to_remove.at(count);
        dataPrivate.data_to_remove  = dataPrivate.data_find;

        create = DataPrivateMuThreadInit(threadData, &dataPrivate, countThread, lenStroke, flag);

        for(tmp = 0; tmp < create; tmp ++){
            pthread_create(&thread[tmp], NULL, functionToCall, &threadData[tmp]);
        }

        for(tmp = 0; tmp < create; tmp ++){
            pthread_join(thread[tmp], NULL);
        }

        if(!isTotal){
            page & p = *dataPrivate.__page;
            QRect area;

            p.removeAt(*dataPrivate.data_to_remove);

            if(stroke_mod.isEmpty())
                continue;

            area = p.get_size_area(*dataPrivate.stroke_mod);

            p.drawSquare(area);
            p.drawIfInside(-1, area);

            dataPrivate.__page->mergeList();
            stroke_mod.clear();

        }

        count ++;
    }

    if(!isTotal)
        data_to_remove.clear();
}
