#include "RubberMethod.h"
#include "touch/TabletUtils.h"
#include "touch/multi_thread_data.h"
#include "touch/rubber/Rubber.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

constexpr bool rubber_debug = false;

struct RubberPrivateData{
    QVector<int>    *data_find;
    Page            *_page;
    WLine           line;
    DataStruct      *data;
    QVector<int>    *data_to_remove;

    int             al_find;

    /**
     * set to true if we need to had remove a stroke with alfa < {max}
     * --> we have to redraw the strokes that were close together
     * */
    bool            highlighter_delete;
    QRect           area;
};

static volatile int     __m_size_gomma;
static WMutex           single_mutex;
static WMutex           mutex_area;
static thread_group_sem *thread_group;

static void (*functionToCall)(DataPrivateMuThread *);

void *idle_rubber(void *arg)
{
    auto *data = (struct DataPrivateMuThread *)arg;

    for(;;){
        thread_group->get_pass_sem().acquire();

        if(un(thread_group->needToDelete())){
            return nullptr;
        }

        if(un(data->id >= thread_group->get_create())){
            goto wait;
        }

        functionToCall(data);
        wait:
        thread_group->get_finish_sem().release();
        thread_group->get_all_finish_sem().acquire();
    }
}

RubberMethod::RubberMethod(const type_rubber &type, const int &size_rubber)
    : _base(-1),
      _size_gomma(size_rubber),
      _rubber_type(type)
{
    WNew(thread_group, thread_group_sem, ());
    thread_group->startLoop(idle_rubber);
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

static force_inline void draw_null(Page *_page, const QVector<int> &point,
                                   const QVector<int> &stroke, bool is_left)
{
    Q_ASSERT(point.size() == stroke.size());

    WCommonScript::for_each(point, stroke, [&_page, is_left](
                                const int indexPoint,
                                const int indexStroke)
    {
        auto &stroke = _page->atStrokeMod(indexStroke);

        W_ASSERT(stroke.type() == Stroke::COMPLEX_NORMAL);

        _page->lock();
        _page->drawForceColorStroke(stroke, -1, COLOR_NULL, nullptr);
        _page->unlock();

        if (is_left) {
            ((StrokeNormal*) &stroke)->removeAt(0, indexPoint);
        } else {
            ((StrokeNormal*) &stroke)->removeAt(indexPoint, -1);
        }

        _page->lock();
        _page->drawStroke(stroke, -1);
        _page->unlock();
    });
}

static bool makeNormal(Page *page, int index, Stroke *stroke)
{
    std::shared_ptr<Stroke> newStroke = stroke->makeNormal();

    if (newStroke) {
        page->swap(index, newStroke);
    }

    return newStroke != nullptr;
}

void actionRubberSinglePartial(DataPrivateMuThread *data)
{
    auto *private_data = (RubberPrivateData *)data->extra;

    QVector<int> stroke_to_remove;
    QVector<int> stroke_mod_point,          stroke_mod_stroke;
    QVector<int> stroke_mod_rigth_point,    stroke_mod_rigth_stroke;
    QVector<int> stroke_mod_left_point,     stroke_mod_left_stroke;

    int from, to, _index;

    Page *_page             = private_data->_page;
    DataStruct *_datastruct = private_data->data;
    const auto &area        = private_data->line;

    from = data->from;
    to = data->to;

    stroke_to_remove.reserve(12);

    W_ASSERT(from <= to);

    for (; from < to; from ++) {
redo:
        Stroke *stroke = &_page->atStrokeMod(from);

        if (un(stroke->isEmpty())) {
            stroke_to_remove.append(from);
            continue;
        }

        _index = 0;
        while (true) {
            int lenPoint;

            const int index = stroke->is_inside(area, _index, __m_size_gomma, true);

            if(index < 0)
                break;

            /**
             * Se lo stroke è diverso da Stroke::COMPLEX_NORMAL la funzione ritornerà
             * true e bisogna richiedere allo stroke che suoi punti dobbiamo togliere.
            */
            if (makeNormal(_page, from, stroke))
                goto redo;

            auto *sNormal = dynamic_cast<StrokeNormal *>(&_page->atStrokeMod(from));

            lenPoint = sNormal->length();
            W_ASSERT(sNormal->type() == Stroke::COMPLEX_NORMAL);

            if (index < 3) {
                if(sNormal->length() - index < 3){
                    stroke_to_remove.append(data->from);

                    // we need to exit the current stroke
                    break;
                }

                W_ASSERT(sNormal->length() >= 2);

                stroke_mod_left_point.append(index);
                stroke_mod_left_stroke.append(from);

                goto out;
            }

            if (index + 3 > lenPoint) {
                if(sNormal->length() < 3)
                    stroke_to_remove.append(data->from);

                stroke_mod_rigth_point.append(index);
                stroke_mod_rigth_stroke.append(from);

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

        for (i-- ; i >= 0; i --) {
            cint indexStroke    = stroke_mod_stroke.at(i);
            cint indexPoint     = stroke_mod_point.at(i);

            _datastruct->changeIdThreadSave(indexPoint, _page->atStrokeMod(indexStroke), *_page);
        }
    }


    single_mutex.lock();

    draw_null(_page, stroke_mod_left_point,  stroke_mod_left_stroke,   true);
    draw_null(_page, stroke_mod_rigth_point, stroke_mod_rigth_stroke,  false);

    // append to the list the index of the stroke to be remove
    private_data->data_to_remove->append(stroke_to_remove);

    single_mutex.unlock();
}

void actionRubberSingleTotal(DataPrivateMuThread *data)
{
    auto *private_data = static_cast<RubberPrivateData *>(data->extra);

    QVector<int> index_selected;
    cint data_already_len   = private_data->al_find;

    Page *_page             = private_data->_page;
    QVector<int> *_al_find  = private_data->data_find;
    const auto &area        = private_data->line;
    index_selected.reserve(32);

    Q_ASSERT(data->from <= data->to);
    W_ASSERT(_page);

    for (; data->from < data->to; data->from++) {
        auto &stroke = _page->atStrokeMod(data->from);
        int index;

        if(WCommonScript::is_present_in_list(_al_find->constData(), data_already_len, data->from))
            continue;

        index = stroke.is_inside(area, 0, __m_size_gomma, false);

        if (index < 0) {
            continue;
        }

        if (un(stroke.is_highlighter())) {
            private_data->highlighter_delete = true;

            const auto currentArea = stroke.getBiggerPointInStroke();

            mutex_area.lock();

            private_data->area = DataStruct::get_bigger_rect(currentArea, private_data->area);

            mutex_area.unlock();
        }

        index_selected.append(data->from);

    }

    if(index_selected.isEmpty()){
        return;
    }

    single_mutex.lock();

    _al_find->append(index_selected);
    private_data->data->decreaseAlfa(index_selected, _page->_count - 1);

    single_mutex.unlock();
}

/**
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
bool RubberMethod::touchUpdate(const QPointF &__lastPoint,
                               double,
                               Document &doc)
{
    int lenStroke, count, indexPage, thread_create;
    cbool isTotal = (_rubber_type == RubberMethod::total);
    const QPointF &lastPoint = doc.adjustPoint(__lastPoint);
    auto *dataThread = thread_group->get_thread_data();
    RubberPrivateData dataPrivate;

    WDebug(true, &this->_last);

    W_ASSERT(_last.isSet());

    functionToCall = (isTotal) ?
            actionRubberSingleTotal :
            actionRubberSinglePartial;

    /**
     * se è la seconda volta che viene chiamata la funzione
     * allora dobbiamo solamente settare _base, altrimenti
     * dobbiamo controllare se si sta rimuovendo qualcosa
     * dalla pagina precedente o dalla successiva
    */
    indexPage = _base;
    count = 0;
    dataPrivate.highlighter_delete = false;
    dataPrivate.area = QRect();

    if (un(_base < 0)) {
        this->_base = doc.whichPage(lastPoint);
        _data_to_remove.append(QVector<int>());
        indexPage = _base;
    } else {
        const auto now = doc.whichPage(lastPoint);
        int i;

        if(likely(now == _base))
            goto out1;

        /**
         * Il punto che ha selezionato adesso l'utente
         * si trova su una pagina precedente a quella che ha
         * selezionato prima
        */
        if (now < _base) {
            for (i = now; i < _base; i++) {
                _data_to_remove.insert(0, QVector<int>());
            }

            _base = now;
            count = 0;
        }
        else if (now > _base) {
            indexPage = now;
            count = now - _base;

            if (!(count < _data_to_remove.length())) {
                for (i = _base; i < now; i++) {
                    _data_to_remove.append(QVector<int>());
                }
            }
        }

        W_ASSERT(now >= 0);
    }

    out1:

    if (!is_image_not_null(
            &doc.at_mod(indexPage),
            lastPoint,
            _last,
            _size_gomma)) {
        WDebug(rubber_debug, "It's null");
        goto save_point;
    }

    WDebug(rubber_debug, "It's not null");

    // l'utente ha prima selezionato un punto su una pagina x,
    // e poi ne ha selezionato un altro su una pagina o x-1, o x+1
    if (un(doc.whichPage(lastPoint) != doc.whichPage(_last))) {
        goto save_point;
    }

    dataPrivate.data       = &doc;
    dataPrivate.line       = WLine(_last, lastPoint);

    __m_size_gomma = _size_gomma;

    dataPrivate._page = &doc.at_mod(indexPage);

    lenStroke = dataPrivate._page->lengthStroke();

    if (un(!lenStroke))
        goto out2;

    // we trigger the copy if the page is shared
    // we can't do after
    dataPrivate._page->atStrokeMod(0);

    dataPrivate.data_find      = &_data_to_remove.operator[](count);
    dataPrivate.data_to_remove = dataPrivate.data_find;
    dataPrivate.al_find        = dataPrivate.data_find->length();

    thread_create = DataPrivateMuThreadInit(dataThread, &dataPrivate,
                                            thread_group->get_max(),
                                            lenStroke, DATA_PRIVATE_FLAG_SEM);

    thread_group->postAndWait(thread_create);

    if (un(dataPrivate.highlighter_delete)) {
        dataPrivate._page->drawIfInside(-1, dataPrivate.area);
    }

    if (!isTotal) {
        dataPrivate._page->removeAt(*dataPrivate.data_to_remove);

        dataPrivate._page->mergeList();
    }

    out2:
    if (!isTotal) {
        _base = -1;
        _data_to_remove.clear();
    }

    save_point:
    _last = lastPoint;
    
    return true;
}

bool RubberMethod::is_image_not_null(const Page *page,
                                     const QPointF &from, const QPointF &to,
                                     int delta)
{
    int i, j;

    W_ASSERT(page);
    W_ASSERT(delta > 0);

    const auto &img = page->getImg();

    const auto ymin = qMin(from.y(), to.y());
    const auto ymax = qMax(from.y(), to.y());
    const auto xmin = qMin(from.x(), to.x());
    const auto xmax = qMax(from.x(), to.x());

    const int x = static_cast<int>(xmin) - delta;
    const int y = static_cast<int>(
                            ymin - Page::getHeight() *
                            (page->getCount() - 1)
                        )
                        - delta;
    W_ASSERT(y >= 0. and y <= Page::getHeight());

    delta *= PROP_RESOLUTION;

    if (un(img.isNull()))
        return false;
    const auto &data = img.toImage();
    for (i = 0; i < delta + int(xmax - xmin); i++) {
        for (j = 0; j < delta + int(ymax - ymin); j++) {
            const QPoint target = QPoint(x + i, y + j) * PROP_RESOLUTION;
            const QRgb pix = data.pixel(target);

            //WDebug(rubber_debug, name << "pixel" << pix);

            if (pix) {
                WDebug(rubber_debug, "yes");
                return true;
            }
        }
    }

    WDebug(rubber_debug, "No");
    return false;
}

int RubberMethod::touchEnd(const QPointF&, Document &doc )
{
    int i, len = _data_to_remove.length();
    int index_mod = -1;

    if (_rubber_type == type_rubber::total) {
        for (i = 0; i < len; i ++) {
            QVector<int> &arr = _data_to_remove.operator[](i);
            Page &page = doc.at_mod(i + _base);

            if(un(arr.isEmpty()))
                continue;

            if(un(index_mod >= 0)){
                index_mod = -2;
            }

            if(un(index_mod == -1)){
                index_mod = i + _base;
            }

            WCommonScript::order_vector(arr);

            const auto rect = doc.get_size_area(arr, i + _base);
            page.removeAndDraw(-1, arr, rect);
        }

        _data_to_remove.clear();
    }

    this->reset();
    return index_mod;
}

RubberMethod::~RubberMethod()
{
    WDelete(thread_group);
}

bool RubberMethod::touchBegin(const QPointF &point, double, Document &doc)
{
    _last = true;
    _last = doc.adjustPoint(point);

    WDebug(true, &this->_last);

    W_ASSERT(_last.isSet());
    return false;
}
