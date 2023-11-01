#include "SquareMethod.h"

#include <utility>
#include "core/core.h"
#include "touch/square/Square.h"
#include "touch/TabletUtils.h"
#include "touch/paintevent/paint.h"
#include "log/log_ui/log_ui.h"
#include "touch/paint.h"

static WMutex           __mutex_sq;
static const Page       *__page;
static PointF          __f;
static PointF          __s;
static WVector<int>     *__index;
static bool             *__in_box;

class SquareTask: public WTask
{
private:
    // TOOD: remove this paramter
    const int _index;
    DataPrivateMuThread & _data;
public:
    explicit SquareTask(int index, DataPrivateMuThread& data)
        : WTask {nullptr, WTask::NotDeleteLater}
        , _index{index}
        , _data{data}
    {

    };

    void run() override;
};

void SquareTask::run()
{
    bool in_box = false;

    W_ASSERT(_data.from <= _data.to);

    for (; _data.from < _data.to;  _data.from ++) {
        const Stroke &stroke = __page->atStroke(_data.from);

        if (datastruct_isinside(__f, __s, stroke)) {
            WMutexLocker<WMutex>::atomically(__mutex_sq, [this] {
                __index->append(_data.from);
            });

            in_box = true;
        }
    }

    if (in_box)
        *__in_box = true;
}

SquareMethod::SquareMethod(Fn<void()> hideProperty,
                           Fn<void (const PointF &, ActionProperty)> showProperty,
                           Fn<Document &()> getDoc)
    : _hideProperty(std::move(hideProperty))
    , _showProperty(std::move(showProperty))
    , _getDoc(std::move(getDoc))
    , _need_reload(false)
    , _base(0)
    , _in_box(false)
{
    _dataThread    = get_data_max();
    _threadCount   = get_thread_used();

    for (int i = 0; i < _threadCount; i++) {
        _dataThread.append(DataPrivateMuThread());
        _tasks.append(Scheduler::Ptr<SquareTask>::make(i, _dataThread[i]));
    }

    WNew(_copy, copy, ());

    _penna.setStyle(WPen::DotLine);
    _penna.setWidthF(2);
    _penna.setColor(WColor::fromRgb(30, 90, 255));
    SquareMethod::reset();

    __in_box = &_in_box;
}

SquareMethod::~SquareMethod()
{
    _tasks.forAll(&Scheduler::Ptr<WTask>::release);
    WDelete(_copy);
}

void SquareMethod::reset()
{
    int i;

    WDebug(debugSquare, "call");
    _pointinit.set(false);
    _lastpoint.set(false);
    _pointfine.set(false);

    _in_box = false;

    _need_reload = false;
    _index_img.clear();

    WDebug(debugSquare, "paste = 1");

    for (i = 0; i < _stroke.size(); i++) {
        WListFast<SharedPtr<Stroke>> ll   = _stroke.operator[](i);

#ifdef DEBUGINFO
        WUtils::for_each(ll, [](const std::shared_ptr<Stroke>& stroke){
            W_ASSERT(!stroke->isEmpty());
        });
#endif // DEBUGINFO

        auto &page = _getDoc()[i + _base];

        for (auto &ref : ll){
            ref->scale(_trans_img);
        }

        page.append(ll);
        page.triggerRenderImage(-1, false);
    }

    _stroke.clear();

    this->_img = WPixmap();
    this->_stroke.clear();
    this->_trans_img = PointF(0.0, 0.0);
}

auto SquareMethod::touchBegin(const PointF &point, double, Document &doc) -> UpdateEvent
{
    constexpr auto not_used debugSquare = false;
    WDebug(debugSquare, "call");

    if(somethingInBox()){
        WDebug(debugSquare, "Somethininbox");
        initPointMove(point, doc);
    }
    else{
        WDebug(debugSquare, "not in box");
        initPointSearch(point, doc);
    }

    return UpdateEvent::makeSquare();
}

auto SquareMethod::touchUpdate(const PointF &point, double, Document &doc) -> UpdateEvent
{
    if (somethingInBox()) {
        W_ASSERT(this->_pointinit.isSet());

        /** a questo punto può muovere di un delta x e y */
        this->move(point, doc);
    } else {
        /**
        * it means that the user not select anything
        * in the past
        */
        this->updatePoint(point, doc);
    }

    return UpdateEvent::makeSquare();
}

auto SquareMethod::touchEnd(const PointF &, class Document &doc) -> UpdateEvent
{
    bool done = somethingInBox();

    if (done) {
        //reset();
    } else {
        find(doc);
    }

    const auto result = endMoving(doc);
    // TODO: adjust
    return UpdateEvent::makePageAll() | UpdateEvent::makeSquare();
}

/**
 * la funzione capisce se all'interno del quadrato
 * della selezione c'è qualcosa in caso salva l'id
 * del tratto e setta la variabile this->check =
 * true, in caso contrario la setta = false e fa il return
*/
bool SquareMethod::find(Document &doc)
{
    bool tmp_find;
    int i, create, lenPage, count;
    int PageCounter;
    WListFast<WVector<int>> index;

    this->adjustPoint();

    const PointF &topLeft = _pointinit;
    const PointF &bottomRight = _pointfine;

    W_ASSERT(topLeft.x() >= 0. && topLeft.y() >= 0.);
    W_ASSERT(bottomRight.x() >= 0. && bottomRight.y() >= 0.);

    WDebug(debugSquare, "call");

    _base = doc.getFirstPageVisible();
    lenPage = doc.lengthPage();
    _in_box = false;
    _need_reload = false;
    PageCounter = _base;

    __f     = topLeft;
    __s     = bottomRight;

    /* point selected by user */
    for (count = 0; PageCounter < lenPage; PageCounter ++, count ++){
        __page = &doc.at(PageCounter);
        create = DataPrivateMuThreadInit(_dataThread, nullptr, _threadCount, __page->lengthStroke(), 0);

        if(__page->isVisible() == false)
            break;

        if(count > index.size() - 1)
            index.append(WVector<int>());

        __index = &index.operator[](count);

        _tasks.refMidConst(0, create).forAll(&Scheduler::addTaskGeneric);
        _tasks.refMidConst(0, create).forAll(&WTask::join);
    }

    /* image selected by user */
    for (int counterImg = 0; counterImg < doc.lengthImage(); counterImg++) {
        const auto &ref = doc.m_img.at(counterImg);

        tmp_find = datastruct_isinside(topLeft, bottomRight, ref.i) ||
                   datastruct_isinside(topLeft, bottomRight, ref.f);

        if (not tmp_find)
            continue;

        _index_img.append(counterImg);
        _in_box = true;
    }

    findObjectToDraw(index, doc);

    moveObjectIntoPrivate(index, doc);

    if(!somethingInBox()){
        if(!_copy->isEmpty()){
            _pointinit = __f;

            _pointinit.set(false);
            _pointfine.set(false);
        }

        reset();
    }else{
        _need_reload = true;
    }

    return _in_box;
}

force_inline void SquareMethod::initImg(const Document &doc)
{
    _img = WPixmap(doc.lengthPage(), true);
}

void SquareMethod::mergeImg(
        const WPixmap    &from,
        WPixmap          &to,
        int              page)
{
    WPainterUnsafe painter;
    auto rectTo = from.rect();

    rectTo.translate(0, page * Page::getResolutionHeigth());

    painter.begin(&to);
    W_ASSERT(painter.isActive());
    painter.drawPixmap(rectTo, from, from.rect());
    painter.end();
}

void SquareMethod::moveObjectIntoPrivate(WListFast<WVector<int>> &index, Document &doc)
{
    int count;
    const auto len = index.size();
    Page * page;
    WPixmap tmp;

    WDebug(debugSquare, "call");

    _stroke.clear();

    for (auto &list: index) {
        WAbstractList::sort(list.begin(), list.end());
    }

    this->initImg(doc);

    auto preappend = [&, len]() {
        for (auto i = 0; i < len; i++) {
            _stroke.append(WListFast<SharedPtr<Stroke>> ());
        }
    };

    preappend();

    for (count = 0; count < len; count ++) {
        const WVector<int> & ref = index.at(count);
        WDebug(debugSquare, ref);

        page = &doc[count + _base];

        if (ref.isEmpty())
            continue;

        page->drawToImage(ref, tmp, DR_IMG_INIT_IMG);

        this->mergeImg(tmp, _img, count + _base);

        page->swap(_stroke.operator[](count), ref, PAGE_SWAP_TRIGGER_VIEW);
    }

#ifdef DEBUGINFO
    WUtils::for_each(_stroke, [](const WListFast<SharedPtr<Stroke>> &list) {
        WUtils::for_each(list, [](const SharedPtr<Stroke>& s) {
            W_ASSERT(!s->isEmpty());
        });
    });
#endif // DEBUGINFO
}

void SquareMethod::findObjectToDrawImg(Document &doc)
{
    for(int index : std::as_const(_index_img)){
        const auto &ref = doc.m_img.at(index);

        if (ref.i.x() < _pointinit.x())
            _pointinit.setX(ref.i.x());
        else if (ref.f.x() > _pointfine.x())
            _pointfine.setX(ref.f.x());

        if (ref.i.y() < _pointinit.y())
            _pointinit.setY(ref.i.y());
        else if (ref.f.y() > _pointfine.y())
            _pointfine.setY(ref.f.y());
    }
}

/**
 * la funzione viene richiamata quando dobbiamo
 * spostare un po' di oggetti nella lista m_id
 * in questo caso si analizza quando c'è un id
 *  uguale, e si sposta tutto il tratto
*/
void SquareMethod::findObjectToDraw(const WListFast<WVector<int>> &index, Document &doc)
{
    const auto trans = doc.getPointFirstPage();
    RectF sizeData;
    return;
    WDebug(debugSquare, "call");

    if (index.isEmpty())
        goto img;

    // find the first point
    sizeData = doc.get_size_area(index, _base);

    _pointinit = sizeData.topLeft() + trans;
    _pointfine = sizeData.bottomRight() + trans;

    img:
    findObjectToDrawImg(doc);
}

void SquareMethod::initPointMove(const PointF &point, const Document &doc)
{
    PointF new_point;
    RectF rect(_pointinit, _pointfine);
    WDebug(debugSquare, "call");

    new_point = doc.adjustPoint(point);

    _lastpoint = PointSettable(point, true);

    WDebug(debugSquare, "initPointMove" << rect.topLeft() << rect.bottomRight() << new_point);

    if (!rect.contains(new_point)) {
        WDebug(debugSquare, "Not in box");
        this->reset();
        this->initPointSearch(point, doc);
    }
}

void SquareMethod::move(const PointF &punto, Document &doc)
{
    PointF delta;
    const auto zoom = doc.getZoom();

    WDebug(debugSquare, "call");

#ifdef DEBUGINFO
    W_ASSERT(somethingInBox());
#else
    if(un(!somethingInBox())){
        NAME_LOG_EXT->write("somethingInBox", log_ui::possible_bug);
        this->changeInstrument();
        return;
    }
#endif

    delta = (_lastpoint - punto) / zoom;

    delta *= -1.;

    _trans_img += delta;

    doc.moveImage(_index_img, delta);

    _pointinit += delta;
    _pointfine += delta;

    _lastpoint = punto;
    _need_reload = true;
}

int SquareMethod::endMoving(Document &doc)
{
    PointF middle;
    const auto ref = doc.getPointFirstPage();
    ActionProperty flag;
    const auto &translation = doc.adjustPointReverce(_pointinit);
    //const Point &translation = -pixmap->mapFromGlobal(Point(0, 0));

    WDebug(debugSquare, "call");

    middle = PointF(translation.x(),
                    translation.y() - 50.);

    flag = this->calculate_flags();

    WDebug(debugSquare, flag << middle );

    this->_showProperty(middle, flag);

    /** for now we say multiple page has changed */
    return -2;
}

void SquareMethod::actionProperty(PropertySignals action)
{
    int flags = 0, dontcall_copy = 1;

    switch (action) {
        case PropertySignals::SignalCopy : {
            flags = SELECTION_FLAGS_COPY;
            break;
        }
        case PropertySignals::SignalCut : {
            flags = SELECTION_FLAGS_CUT;
            break;
        }
        case PropertySignals::SignalPaste: {
            flags = SELECTION_FLAGS_PASTE;
            break;
        }
        case PropertySignals::SignalDelete: {
            _stroke.clear();

            //data.removePointIndex(index, base, true);
            dontcall_copy = 0;
            this->_hideProperty();
            _stroke.clear();
            this->reset();
            break;
        }
        default:{
            NAME_LOG_EXT->write(
                    WString("It was not possibile to determinate %1").arg(WString::number((int)action)),
                    log_ui::error_internal);
            WUtils::abortIfDebug(__FUNCTION__, __LINE__);
        }
    }

    if (dontcall_copy) {
        if (_copy->selection(_getDoc(), _stroke, flags, _pointinit)) {
            _stroke.clear();
            this->_hideProperty();
            this->reset();
        }
    } else {
        this->reset();
    }

    this->needRefreshPrivate(UpdateEvent::makeSquare() | UpdateEvent::makePage(this->_base, _base + this->_stroke.size() - 1));
}

/**
 * la funcione cambia i punti, in caso l'utente non abbia tracciato il
 * rettangolo da in alto a sinistra a in alto a destra
*/
force_inline void SquareMethod::adjustPoint()
{
    PointF &topLeft        = _pointinit;
    PointF &bottomRight    = _pointfine;

    WDebug(debugSquare, topLeft << bottomRight);

    if(topLeft.x() > bottomRight.x())
        WUtils::swap(topLeft.rx(), bottomRight.rx());

    if(topLeft.y() > bottomRight.y())
        WUtils::swap(topLeft.ry(), bottomRight.ry());

    WDebug(debugSquare, topLeft << bottomRight);

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());
}

static void square_draw_square(
        WPainter            &painter,
        const DataStruct    &data,
        const PointF       &tl,
        const PointF       &br)
{
    const PointF TL = data.adjustPointReverce(tl);
    const PointF BR = data.adjustPointReverce(br);
    constexpr const auto debugDraw = true;

    WDebug(debugSquare and debugDraw, tl << br << TL << BR);

    painter.drawRect(RectF(TL, BR));
}

void SquareMethod::needReload(WPainter &painter, const Document &doc)
{
    if constexpr (WUtils::debug_enable()) {
        if (!painter.isActive()) {
            WWarning("Painter not active in Square");
            std::abort();
        }
    }

    if (_need_reload) {
        const auto zoom = doc.getZoom();
        WDebug(debugSquare, "__need_reload = true");

        if (somethingInBox()){
            const PointF point = doc.getPointFirstPage() + _trans_img * zoom;
            const int len = doc.lengthPage();
            const WSizeF size = createSizeRect(doc, len, DRAW_CREATE_SIZE_RECT_DEF_PRO);

            W_ASSERT(size.getHeight() >= 0 and size.getWidth() >= 0);
            WDebug(debugSquare, "in_box");

            singleLoad(painter, _img, size, point,
                       0, DRAW_SINGLE_LOAD_DEF);
        }

        painter.setPen(_penna);
        square_draw_square(painter, doc, _pointinit, _pointfine);
    }
}

void SquareMethod::updatePoint(const PointF &puntofine, const Document &doc)
{
    WDebug(debugSquare, "call");
    W_ASSERT(!somethingInBox());

    _pointfine = doc.adjustPoint(puntofine);

    W_ASSERT(_pointfine.x() >= 0.0 and _pointinit.y() >= 0.0);

    _pointfine = true;

    _need_reload = true;
}

void SquareMethod::initPointSearch(const PointF &point, const Document &doc)
{
    WDebug(debugSquare, "call");

    W_ASSERT(!somethingInBox());

    _pointinit = doc.adjustPoint(point);
    W_ASSERT(_pointinit.x() >= 0.0 && _pointinit.y() >= 0.0);
    _pointinit.set(true);

    /** we don't need yet to draw somethings */
    _need_reload = false;
    _in_box = false;

    this->_hideProperty();
}
