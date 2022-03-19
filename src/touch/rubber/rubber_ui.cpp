#include "rubber_ui.h"
#include "ui_rubber_ui.h"
#include "utils/common_script.h"
#include "pthread.h"
#include "semaphore.h"
#include "touch/multi_thread_data.h"
#include "core/wmultiplemutex.h"
#include "core/wline.h"
#include "mainwindow.h"

struct RubberPrivateData{
    QVector<int>    *data_find;
    page            *__page;
    WLine           line;
    datastruct      *data;
    QVector<int>    *data_to_remove;

    int             al_find;
};

static volatile int     __m_size_gomma;
static pthread_mutex_t  single_mutex;
static thread_group_sem *thread_group;

static void (*functionToCall)(DataPrivateMuThread *);

void *idle_rubber(void *arg)
{
    DataPrivateMuThread *data = (struct DataPrivateMuThread *)arg;

    for(;;){
        sem_wait(thread_group->get_pass_sem());

        if(unlikely(thread_group->needToDelete())){
            return NULL;
        }

        if(unlikely(data->id >= thread_group->get_create())){
            goto wait;
        }

        functionToCall(data);
wait:
        sem_post(thread_group->get_finish_sem());
        sem_wait(thread_group->get_all_finish_sem());
    }
}

rubber_ui::rubber_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::rubber_ui)
{
    _canvas = ((MainWindow *)parent)->_canvas;
    ui->setupUi(this);

    this->load_settings();

    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);

    ui->totale_button->setCheckable(true);
    ui->partial_button->setCheckable(true);

    pthread_mutex_init(&single_mutex, NULL);
    thread_group = new thread_group_sem;
    thread_group->startLoop(idle_rubber);

    this->reset();
}

rubber_ui::~rubber_ui()
{
    this->save_settings();
    delete thread_group;

    pthread_mutex_destroy(&single_mutex);
    delete ui;
}

bool rubber_ui::event(QEvent *event)
{
    if(unlikely(event->type() == QEvent::WindowDeactivate))
        this->hide();

    return QWidget::event(event);
}

void rubber_ui::on_totale_button_clicked()
{
    this->_type_gomma = e_type_rubber::total;

    this->update_data();
}

void rubber_ui::on_partial_button_clicked()
{
    _type_gomma = e_type_rubber::partial;

    this->update_data();
}

void rubber_ui::endRubber()
{
    datastruct *data = _canvas->data->datatouch;
    int i, len = _data_to_remove.length();

    W_ASSERT(data);

    if(_type_gomma == e_type_rubber::total){

        for(i = 0; i < len; i ++){
            QVector<int> &arr = _data_to_remove.operator[](i);
            page &page = data->at_mod(i + _base);

            if(unlikely(arr.isEmpty()))
                continue;

            order(arr);

            const auto rect = data->get_size_area(arr, i + _base);
            page.removeAndDraw(-1, arr, rect);
        }

        _data_to_remove.clear();
    }

    this->reset();
}

static inline not_used QRectF rubber_get_area(const QPointF &p1, const QPointF &p2)
{
    const auto &size = __m_size_gomma;

    QRectF area = datastruct_rect(p1, p2);
    const QPointF tmp = QPointF(size, size);

    W_ASSERT(__m_size_gomma >= 0);
    W_ASSERT(area.topLeft().x() >= 0.);
    W_ASSERT(area.topLeft().y() >= 0.);

    area = QRectF(area.topLeft() - tmp,
                  area.bottomRight() + tmp);

    return area;
}

static force_inline void draw_null(page *_page, const QVector<int> &point,
                                   const QVector<int> &stroke, bool is_left)
{
    int i, len;

    len = point.length();
    Q_ASSERT(point.size() == stroke.size());

    for(i = 0; i < len; i++){
        cint indexPoint  = point.at(i);
        cint indexStroke = stroke.at(i);

        auto &stroke = _page->atStrokeMod(indexStroke);
        const auto len = stroke.length();

        _page->lock();
        _page->drawForceColorStroke(stroke, -1, COLOR_NULL, NULL);
        _page->unlock();

        if(is_left){
            stroke.removeAt(0, indexPoint);
        }else{
            stroke.removeAt(indexPoint, len - 1);
        }

        _page->lock();
        _page->drawStroke(stroke, -1);
        _page->unlock();
    }
}

void actionRubberSinglePartial(DataPrivateMuThread *data)
{
    RubberPrivateData *private_data = (RubberPrivateData *)data->extra;

    QVector<int> stroke_to_remove;
    QVector<int> stroke_mod_point,          stroke_mod_stroke;
    QVector<int> stroke_mod_rigth_point,    stroke_mod_rigth_stroke;
    QVector<int> stroke_mod_left_point,     stroke_mod_left_stroke;

    int from, to, _index, index;

    page *_page             = private_data->__page;
    datastruct *_datastruct = private_data->data;
    const auto &area        = private_data->line;

    from = data->from;
    to = data->to;

    stroke_to_remove.reserve(12);

    W_ASSERT(from <= to);

    for(; from < to; from ++){
        stroke & stroke = _page->atStrokeMod(from);

        if(unlikely(stroke.isEmpty())){
            stroke_to_remove.append(from);
            continue;
        }

        _index = 0;
        while(true){
            int lenPoint;
            index = stroke.is_inside(area, _index, __m_size_gomma, true);

            if(index < 0)
                break;

            lenPoint = stroke.length();
            W_ASSERT(stroke.is_normal());

            if(index < 3){
                if(stroke.length() - index < 3){
                    stroke_to_remove.append(data->from);

                    // we need to exit the current stroke
                    break;
                }

                W_ASSERT(stroke.length() >= 2);

                stroke_mod_left_point.append(index);
                stroke_mod_left_stroke.append(from);
                //stroke.removeAt(0, index);

                goto out;
            }

            if(index + 3 > lenPoint){
                if(stroke.length() < 3)
                    stroke_to_remove.append(data->from);

                stroke_mod_rigth_point.append(index);
                stroke_mod_rigth_stroke.append(from);
                //stroke.removeAt(index, lenPoint - 1);

                break;
            }

            W_ASSERT(lenPoint >= 6);

            stroke_mod_point.append(index);
            stroke_mod_stroke.append(from);

out:
            _index = index + 1;
        }
    }

    // we don't need to do this operation
    // in order to the list
    {
        int i = stroke_mod_stroke.length();
        W_ASSERT(stroke_mod_stroke.size() == stroke_mod_point.length());

        for(i-- ; i >= 0; i --){
            cint indexStroke    = stroke_mod_stroke.at(i);
            cint indexPoint     = stroke_mod_point.at(i);

            _datastruct->changeIdThreadSave(indexPoint, _page->atStrokeMod(indexStroke), *_page);
        }
    }


    pthread_mutex_lock(&single_mutex);

    draw_null(_page, stroke_mod_left_point,  stroke_mod_left_stroke,   true);
    draw_null(_page, stroke_mod_rigth_point, stroke_mod_rigth_stroke,  false);

    // append to the list the index of the stroke to be remove
    private_data->data_to_remove->append(stroke_to_remove);

    pthread_mutex_unlock(&single_mutex);

}

void actionRubberSingleTotal(DataPrivateMuThread *data)
{
    RubberPrivateData *private_data = (RubberPrivateData *)data->extra;

    QVector<int> index_selected;
    cint data_already_len   = private_data->al_find;

    page *_page             = private_data->__page;
    QVector<int> *_al_find  = private_data->data_find;
    const auto &area        = private_data->line;
    index_selected.reserve(32);

    Q_ASSERT(data->from <= data->to);

    for(; data->from < data->to; data->from++){
        stroke &__stroke = _page->atStrokeMod(data->from);
        int index;

        if(is_present_in_list(_al_find->constData(), data_already_len, data->from))
            continue;

        index = __stroke.is_inside(area, 0, __m_size_gomma, false);

        if(index < 0){
            continue;
        }

        index_selected.append(data->from);

    }

    if(index_selected.isEmpty()){
        return;
    }

    pthread_mutex_lock(&single_mutex);

    _al_find->append(index_selected);
    private_data->data->decreaseAlfa(index_selected, _page->_count - 1);

    pthread_mutex_unlock(&single_mutex);

}

#define PrivateData(attribute) dataPrivate.attribute

/*
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
void rubber_ui::actionRubber(const QPointF &__lastPoint)
{
    datastruct *data = _canvas->data->datatouch;
    int lenStroke, count, indexPage, thread_create;
    const bool isTotal = (_type_gomma == e_type_rubber::total);
    const QPointF &lastPoint = data->adjustPoint(__lastPoint);
    auto *dataThread = thread_group->get_thread_data();
    RubberPrivateData dataPrivate;

    W_ASSERT(_last.set);

    if(isTotal){
        functionToCall = actionRubberSingleTotal;
    }else{
        functionToCall = actionRubberSinglePartial;
    }

    /* se è la seconda volta che viene chiamata la funzione
    * allora dobbiamo solamente settare _base, altrimenti
    * dobbiamo controllare se si sta rimuovendo qualcosa
    * dalla pagina precedente o dalla successiva
    */
    indexPage = _base;
    count = 0;

    if(unlikely(_base < 0)){
        this->_base = data->whichPage(lastPoint);
        _data_to_remove.append(QVector<int>());
        indexPage = _base;
    }else{
        const auto now = data->whichPage(lastPoint);

        if(now < _base){
            _base = now;

            for(int i = now; i < _base; i++){
                _data_to_remove.insert(0, QVector<int>());
            }

            _data_to_remove.insert(0, QVector<int>());
            count = 0;
        }else if(now > _base){

            for(int i = _base; i < now; i++){
                _data_to_remove.append(QVector<int>());
            }

            indexPage = now;
            count = now - _base;
        }
    }

    // l'utente ha prima selezionato un punto su una pagina x,
    // e poi ne ha selezionato un altro su una pagina o x-1, o x+1
    if(unlikely(_base != data->whichPage(_last.point))){
        return;
    }

    PrivateData(data)       = data;
    PrivateData(line)       = WLine(_last.point, lastPoint);

    __m_size_gomma = _size_gomma;

    dataPrivate.__page = &data->at_mod(indexPage);

    lenStroke = dataPrivate.__page->lengthStroke();

    // we trigger the copy if the page is shared
    if(unlikely(!lenStroke))
        goto out;

    dataPrivate.__page->atStrokeMod(0);

    PrivateData(data_find)      = &_data_to_remove.operator[](count);
    PrivateData(data_to_remove) = dataPrivate.data_find;
    PrivateData(al_find)        = dataPrivate.data_find->length();

    thread_create = DataPrivateMuThreadInit(dataThread, &dataPrivate,
                                            thread_group->get_max(), lenStroke, DATA_PRIVATE_FLAG_SEM);

    thread_group->postAndWait(thread_create);

    if(!isTotal){
        page & p = *dataPrivate.__page;

        p.removeAt(*dataPrivate.data_to_remove);

        dataPrivate.__page->mergeList();

    }
out:
    if(!isTotal){
        _base = -1;
        _data_to_remove.clear();
    }

    _last.point = lastPoint;
}

void rubber_ui::initRubber(const QPointF &point)
{
    datastruct *data = _canvas->data->datatouch;
    _last.set = true;
    _last.point = data->adjustPoint(point);
}
