#include "RubberMethod.h"
#include "touch/TabletUtils.h"
#include "touch/multi_thread_data.h"
#include "touch/rubber/Rubber.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"
#include "core/WRgb.h"

struct RubberPrivateData {
    WVector<int>    *data_find;
    Page            *_page;
    WLine           line;
    DataStruct      *data;
    WVector<int>    *data_to_remove;

    int             al_find;

    /**
     * set to true if we need to had remove a stroke with alfa < {max}
     * --> we have to redraw the strokes that were close together
     * */
    bool            highlighter_delete;
    RectF           area;
};

static volatile int     __m_size_gomma;
static WMutex           single_mutex;
static WMutex           mutex_area;

class RubberMethodTask: public WTask
{
private:
    void (**_functionToCall)(DataPrivateMuThread &);
    DataPrivateMuThread &_data;
public:
    explicit RubberMethodTask(void (**functionToCall)(DataPrivateMuThread &), DataPrivateMuThread& data);
    void run() final
    {
        (*_functionToCall)(_data);
    };
};

RubberMethodTask::RubberMethodTask(void (**functionToCall)(DataPrivateMuThread &), DataPrivateMuThread& data)
    : WTask{nullptr, WTask::NotDeleteLater}
    , _functionToCall{functionToCall}
    , _data{data}
{

}

RubberMethod::RubberMethod(const type_rubber &type, const int &size_rubber)
    : _base(-1)
    , functionToCall(nullptr)
    , _size_gomma(size_rubber)
    , _rubber_type(type)
{
    const auto numberOfThreads = threadCount::count();

    for (int i = 0; i < numberOfThreads; i++) {
        _dataTasks.append(DataPrivateMuThread());
        _tasks.append(Scheduler::Ptr<RubberMethodTask>::make(&this->functionToCall, _dataTasks[i]));
    }
}

static inline not_used RectF rubber_get_area(const PointF &p1, const PointF &p2)
{
    const auto &size = __m_size_gomma;

    RectF area = datastruct_rect(p1, p2);
    const PointF tmp = PointF(size, size);

    W_ASSERT(__m_size_gomma >= 0);
    W_ASSERT(area.topLeft().x() >= 0.);
    W_ASSERT(area.topLeft().y() >= 0.);

    area = RectF(area.topLeft() - tmp,
                  area.bottomRight() + tmp);

    return area;
}

static force_inline void draw_null(Page &_page, const WVector<int> &point,
                                   const WVector<int> &stroke, bool is_left)
{
    W_ASSERT(point.size() == stroke.size());

    WUtils::for_each(point, stroke, [&_page, is_left](
                                const int indexPoint,
                                const int indexStroke)
    {
        auto &stroke = _page[indexStroke];

        W_ASSERT(stroke.type() == Stroke::COMPLEX_NORMAL);

        _page.lock();
        _page.drawForceColorStroke(stroke, -1, COLOR_NULL, nullptr);
        _page.unlock();

        if (is_left) {
            ((StrokeNormal*) &stroke)->removeAt(0, indexPoint);
        } else {
            ((StrokeNormal*) &stroke)->removeAt(indexPoint, -1);
        }

        _page.lock();
        _page.drawStroke(stroke, -1);
        _page.unlock();
    });
}

static bool makeNormal(Page &page, int index, Stroke &stroke)
{
    std::shared_ptr<Stroke> newStroke = stroke.makeNormal();

    if (newStroke) {
        page.swap(index, newStroke);
    }

    return newStroke != nullptr;
}

void actionRubberSinglePartial(DataPrivateMuThread &data)
{
    auto *private_data = (RubberPrivateData *)data.extra;

    WVector<int> stroke_to_remove;
    WVector<int> stroke_mod_point,          stroke_mod_stroke;
    WVector<int> stroke_mod_rigth_point,    stroke_mod_rigth_stroke;
    WVector<int> stroke_mod_left_point,     stroke_mod_left_stroke;

    int from, to, _index;

    Page &_page             = *private_data->_page;
    DataStruct &_datastruct = *private_data->data;
    const auto &area        = private_data->line;

    from = data.from;
    to = data.to;

    stroke_to_remove.reserve(12);

    W_ASSERT(from <= to);

    for (; from < to; from ++) {
redo:
        Stroke &stroke = _page[from];

        if (stroke.isEmpty()) {
            stroke_to_remove.append(from);
            continue;
        }

        _index = 0;
        while (true) {
            int lenPoint;

            const int index = stroke.is_inside(area, _index, __m_size_gomma, true);

            if(index < 0)
                break;

            /**
             * Se lo stroke è diverso da Stroke::COMPLEX_NORMAL la funzione ritornerà
             * true e bisogna richiedere allo stroke che suoi punti dobbiamo togliere.
            */
            if (makeNormal(_page, from, stroke))
                goto redo;

            auto &sNormal = dynamic_cast<StrokeNormal &>(_page[from]);

            lenPoint = sNormal.length();
            W_ASSERT(sNormal.type() == Stroke::COMPLEX_NORMAL);

            if (index < 3) {
                if(sNormal.length() - index < 3){
                    stroke_to_remove.append(data.from);

                    // we need to exit the current stroke
                    break;
                }

                W_ASSERT(sNormal.length() >= 2);

                stroke_mod_left_point.append(index);
                stroke_mod_left_stroke.append(from);

                goto out;
            }

            if (index + 3 > lenPoint) {
                if(sNormal.length() < 3)
                    stroke_to_remove.append(data.from);

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
        auto i = stroke_mod_stroke.size();
        W_ASSERT(stroke_mod_stroke.size() == stroke_mod_point.size());

        for (i-- ; i >= 0; i --) {
            cint indexStroke = stroke_mod_stroke.at(i);
            cint indexPoint  = stroke_mod_point.at(i);

            _datastruct.changeIdThreadSave(indexPoint, _page[indexStroke], _page);
        }
    }


    {
        WMutexLocker guard(single_mutex);

        draw_null(_page, stroke_mod_left_point,  stroke_mod_left_stroke,   true);
        draw_null(_page, stroke_mod_rigth_point, stroke_mod_rigth_stroke,  false);

        // append to the list the index of the stroke to be remove
        private_data->data_to_remove->append(stroke_to_remove);
    }
}

void actionRubberSingleTotal(DataPrivateMuThread &data)
{
    auto *private_data = static_cast<RubberPrivateData *>(data.extra);

    WVector<int> index_selected;
    cint data_already_len   = private_data->al_find;

    Page &_page             = *private_data->_page;
    WVector<int> &_al_find  = *private_data->data_find;
    const auto &area        = private_data->line;
    index_selected.reserve(32);

    W_ASSERT(data.from <= data.to);

    for (; data.from < data.to; data.from++) {
        auto &stroke = _page[data.from];
        int index;

        if (_al_find.contains(data.from))
            continue;

        index = stroke.is_inside(area, 0, __m_size_gomma, false);

        if (index < 0) {
            continue;
        }

        if (stroke.is_highlighter()) {
            private_data->highlighter_delete = true;

            const auto currentArea = stroke.getBiggerPointInStroke();

            WMutexLocker guard(mutex_area);
            private_data->area = DataStruct::joinRect(currentArea, private_data->area);
        }

        index_selected.append(data.from);

    }

    if(index_selected.isEmpty()){
        return;
    }

    WMutexLocker guard(single_mutex);

    _al_find.append(index_selected);
    private_data->data->decreaseAlfa(index_selected, _page.getCount() - 1);
}

/**
 * this function is call by tabletEvent
 * it returns true if it actually deleted something, otherwise it returns false
*/
auto RubberMethod::touchUpdate(const PointF &__lastPoint,
                               double,
                               Document &doc) -> UpdateEvent
{
    int lenStroke, count, indexPage, thread_create;
    const bool isTotal = (_rubber_type == RubberMethod::total);
    const PointF &lastPoint = doc.adjustPoint(__lastPoint);
    int pageMod = 0;
    RubberPrivateData dataPrivate;

    WDebug(debug, &this->_last);

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
    dataPrivate.area = RectF();

    if (_base < 0) {
        this->_base = doc.whichPage(lastPoint);
        _data_to_remove.append(WVector<int>());
        indexPage = _base;
    } else {
        const auto now = doc.whichPage(lastPoint);
        int i;

        if (now == _base)
            goto out1;

        /**
         * Il punto che ha selezionato adesso l'utente
         * si trova su una pagina precedente a quella che ha
         * selezionato prima
        */
        if (now < _base) {
            for (i = now; i < _base; i++) {
                _data_to_remove.insert(0, WVector<int>());
            }

            _base = now;
            count = 0;
        } else if (now > _base) {
            indexPage = now;
            count = now - _base;

            if (!(count < _data_to_remove.size())) {
                for (i = _base; i < now; i++) {
                    _data_to_remove.append(WVector<int>());
                }
            }
        }

        W_ASSERT(now >= 0);
    }

    out1:

    if (!is_image_not_null(
            doc[indexPage],
            lastPoint,
            _last,
            _size_gomma)) {
        WDebug(debug, "It's null");
        goto save_point;
    }

    WDebug(debug, "It's not null");

    // l'utente ha prima selezionato un punto su una pagina x,
    // e poi ne ha selezionato un altro su una pagina o x-1, o x+1
    if (doc.whichPage(lastPoint) != doc.whichPage(_last)) {
        goto save_point;
    }

    dataPrivate.data       = &doc;
    dataPrivate.line       = WLine(_last, lastPoint);

    __m_size_gomma = _size_gomma;

    dataPrivate._page = &doc[indexPage];

    lenStroke = dataPrivate._page->lengthStroke();

    if (lenStroke == 0)
        goto out2;

    // we trigger the copy if the page is shared
    // we can't do after
    dataPrivate._page[0];

    dataPrivate.data_find      = &_data_to_remove.operator[](count);
    dataPrivate.data_to_remove = dataPrivate.data_find;
    dataPrivate.al_find        = dataPrivate.data_find->size();

    thread_create = DataPrivateMuThreadInit(_dataTasks, &dataPrivate, _tasks.size(), lenStroke, DATA_PRIVATE_FLAG_SEM);

    _tasks.refMidConst(0, thread_create).forAll(&Scheduler::addTaskGeneric);
    _tasks.refMidConst(0, thread_create).forAll(&WTask::join);

    if (dataPrivate.highlighter_delete) {
        dataPrivate._page->drawIfInside(-1, dataPrivate.area);
    }

    if (!isTotal) {
        dataPrivate._page->removeAt(dataPrivate.data_to_remove->begin(), dataPrivate.data_to_remove->end());

        dataPrivate._page->mergeList();
    }

    out2:
    pageMod = _base;
    if (!isTotal) {
        _base = -1;
        _data_to_remove.clear();
    }

    save_point:
    _last = lastPoint;

    return UpdateEvent::makePage(pageMod, pageMod + 1);
}

bool RubberMethod::is_image_not_null(const Page &page,
                                     const PointF &from, const PointF &to,
                                     int delta)
{
    int i, j;

    W_ASSERT(delta > 0);

    const auto &img = page.getImg();

    const auto ymin = std::min(from.y(), to.y());
    const auto ymax = std::min(from.y(), to.y());
    const auto xmin = std::min(from.x(), to.x());
    const auto xmax = std::min(from.x(), to.x());

    const int x = static_cast<int>(xmin) - delta;
    const int y = static_cast<int>(
                            ymin - Page::getHeight() *
                            (page.getIndex())
                        )
                        - delta;
    W_ASSERT(y >= 0. and y <= Page::getHeight());

    delta *= PROP_RESOLUTION;

    if (img.isNull())
        return false;

    const auto data = img.toImage();
    for (i = 0; i < delta + int(xmax - xmin); i++) {
        for (j = 0; j < delta + int(ymax - ymin); j++) {
            const WPoint target = WPoint(x + i, y + j) * PROP_RESOLUTION;
            const WRgb pix = data.pixel(target);

            //WDebug(rubber_debug, name << "pixel" << pix);

            if (pix) {
                WDebug(debug, "yes");
                return true;
            }
        }
    }

    WDebug(debug, "No");
    return false;
}

auto RubberMethod::touchEnd(const PointF&, Document &doc) -> UpdateEvent
{
    int i, len = _data_to_remove.size();
    int index_mod = -1;

    if (_rubber_type == type_rubber::total) {
        for (i = 0; i < len; i ++) {
            WVector<int> &arr = _data_to_remove.operator[](i);
            Page &page = doc[i + _base];

            if (arr.isEmpty())
                continue;

            if (index_mod >= 0) {
                index_mod = -2;
            }

            if (index_mod == -1) {
                index_mod = i + _base;
            }

            WAbstractList::sort(arr.begin(), arr.end());

            const auto rect = doc.getSizeArea(arr, i + _base);
            page.removeAndDraw(-1, arr, rect);
        }

        _data_to_remove.clear();
    }

    this->reset();
    return UpdateEvent::makePage(index_mod, index_mod + 1);
}

RubberMethod::~RubberMethod()
{
    _tasks.forAll(&Scheduler::Ptr<WTask>::release);
}

auto RubberMethod::touchBegin(const PointF &point, double, Document &doc) -> UpdateEvent
{
    _last = true;
    _last = doc.adjustPoint(point);

    WDebug(debug, &this->_last);

    W_ASSERT(_last.isSet());
    return UpdateEvent::makeEmpty();
}
