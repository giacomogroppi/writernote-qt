#include "SquareMethod.h"
#include "core/core.h"
#include "touch/square/Square.h"
#include "touch/TabletUtils.h"
#include "touch/paintevent/paint.h"
#include "log/log_ui/log_ui.h"

static WMutex           __mutex_sq;
static const Page       *__page;
static PointF          __f;
static PointF          __s;
static WVector<int>     *__index;
static bool             *__in_box;

SquareMethod::SquareMethod(std::function<void()> hideProperty,
                           std::function<void (const PointF &, ActionProperty)> showProperty,
                           std::function<Document &()> getDoc)
    : _hideProperty(hideProperty)
    , _showProperty(std::move(showProperty))
    , _getDoc(getDoc)
    , _need_reload(false)
    , _in_box(false)
{
    _thread        = get_thread_max();
    _dataThread    = get_data_max();
    _threadCount   = get_thread_used();

    WNew(_copy, copy, ());

    _penna.setStyle(Qt::DotLine);
    _penna.setWidth(2);
    _penna.setColor(colore_s::fromRgb(30, 90, 255));
    SquareMethod::reset();

    __in_box = &_in_box;
}

SquareMethod::~SquareMethod()
{
    free_thread_data(&_thread, &_dataThread);
    WDelete(_copy);
}

void SquareMethod::reset()
{
    int i;

    WDebug(debugSquare, "call");
    _pointinit.setSet(false);
    _lastpoint.setSet(false);
    _pointfine.setSet(false);

    _in_box = false;

    _need_reload = false;
    _index_img.clear();

    WDebug(debugSquare, "paste = 1");
    const auto len = _stroke.length();

    if(len == 0)
        goto out;

    for (i = 0; i < len; i++) {
        WListFast<std::shared_ptr<Stroke>> ll   = _stroke.operator[](i);

#ifdef DEBUGINFO
        WCommonScript::for_each(ll, [](const std::shared_ptr<Stroke>& stroke){
            W_ASSERT(!stroke->isEmpty());
        });
#endif // DEBUGINFO

        Page * page         = &_getDoc().at_mod(i + _base);

        for (auto &ref : ll){
            ref->scale(_trans_img);
        }

        page->append(ll);
        page->triggerRenderImage(-1, false);
    }

    _stroke.clear();

    out:
    this->_img = WPixmap();
    this->_stroke.clear();
    this->_trans_img = PointF(0.0, 0.0);
}

void * __square_search(void *__data)
{
    W_ASSERT(__data);
    auto *data = static_cast<DataPrivateMuThread *>(__data);
    bool in_box = false;

    assert(data->from <= data->to);

    for (; data->from < data->to;  data->from ++) {
        const Stroke &stroke = __page->atStroke(data->from);

        if (datastruct_isinside(__f, __s, stroke)) {
            __mutex_sq.lock();
            __index->append(data->from);
            __mutex_sq.unlock();

            in_box = true;
        }
    }

    if (in_box)
        *__in_box = true;

    return nullptr;
}

int SquareMethod::touchBegin(const PointF &point, double, Document &doc)
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

    return true;
}

int SquareMethod::touchUpdate(const PointF &point, double, Document &doc)
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

    return true;
}

int SquareMethod::touchEnd(const PointF &, class Document &doc)
{
    bool done = somethingInBox();

    if (done) {
        //reset();
    } else {
        find(doc);
    }

    return endMoving(doc);
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

#define CTRL_POINT(point) W_ASSERT(point.x() >= 0. && point.y() >= 0.);
    CTRL_POINT(topLeft);
    CTRL_POINT(bottomRight);

    WDebug(debugSquare, "call");

    _base = doc.getFirstPageVisible();
    lenPage = doc.lengthPage();
    _in_box = false;
    _need_reload = false;
    PageCounter = _base;

    __f     = topLeft;
    __s     = bottomRight;

    /* point selected by user */
    for(count = 0; PageCounter < lenPage; PageCounter ++, count ++){
        __page = &doc.at(PageCounter);
        create = DataPrivateMuThreadInit(_dataThread, nullptr, _threadCount, __page->lengthStroke(), 0);

        if(un(!__page->isVisible()))
            break;

        if(likely(count > index.length() - 1))
            index.append(WVector<int>());

        __index = &index.operator[](count);

        for(i = 0; i < create; i++){
            pthread_create(&_thread[i], nullptr, __square_search, (void *)&_dataThread[i]);
        }
        for(i = 0; i < create; i++){
            pthread_join(_thread[i], nullptr);
        }
    }

    /* image selected by user */
    const int lenImg = doc.length_img();
    for (int counterImg = 0; counterImg < lenImg; counterImg++) {
        const auto &ref = doc.m_img.at(counterImg);

        tmp_find = datastruct_isinside(topLeft, bottomRight, ref.i) ||
                   datastruct_isinside(topLeft, bottomRight, ref.f);

        if(!tmp_find)
            continue;

        _index_img.append(counterImg);
        _in_box = true;
    }

    findObjectToDraw(index, doc);

    moveObjectIntoPrivate(index, doc);

    if(!somethingInBox()){
        if(!_copy->isEmpty()){
            _pointinit = __f;

            _pointinit.setSet(false);
            _pointfine.setSet(false);
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
    WPainter painter;
    Rect rectTo = from.rect();

    rectTo.translate(0, page * Page::getResolutionHeigth());

    painter.begin(&to);
    W_ASSERT(painter.isActive());
    painter.drawPixmap(rectTo, from, from.rect());
    painter.end();
}

void SquareMethod::moveObjectIntoPrivate(WListFast<WVector<int>> &index, Document &doc)
{
    int count;
    const auto len = index.length();
    Page * page;
    WPixmap tmp;

    WDebug(debugSquare, "call");

    _stroke.clear();

    WCommonScript::order_multiple(index);

    this->initImg(doc);

    auto preappend = [&, len]() {
        for (auto i = 0; i < len; i++) {
            _stroke.append(WListFast<std::shared_ptr<Stroke>> ());
        }
    };

    preappend();

    for (count = 0; count < len; count ++) {
        const WVector<int> & ref = index.at(count);
        WDebug(debugSquare, ref);
        page = &doc.at_mod(count + _base);

        if (ref.isEmpty())
            continue;

        WDebug(true, "lenPrima" << page->lengthStroke());

        page->drawToImage(ref, tmp, DR_IMG_INIT_IMG);

        this->mergeImg(tmp, _img, count + _base);

        page->swap(_stroke.operator[](count), ref, PAGE_SWAP_TRIGGER_VIEW);
        WDebug(true, "lenDopo" << page->lengthStroke());
    }

#ifdef DEBUGINFO
    WCommonScript::for_each(_stroke, [](const WListFast<std::shared_ptr<Stroke>> &list) {
        WCommonScript::for_each(list, [](const std::shared_ptr<Stroke>& s) {
            W_ASSERT(!s->isEmpty());
        });
    });
#endif // DEBUGINFO
}

void SquareMethod::findObjectToDrawImg(Document &doc)
{
    for(int index : qAsConst(_index_img)){
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

    if(un(index.isEmpty()))
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

    DataStruct::inverso(delta);

    _trans_img += delta;

    doc.moveImage(_index_img, delta);

    _pointinit += delta;
    _pointfine += delta;

    _lastpoint = punto;
    _need_reload = true;
}

int SquareMethod::endMoving(Document &doc)
{
    Point middle;
    const auto ref = doc.getPointFirstPage();
    ActionProperty flag;
    const auto &translation = doc.adjustPointReverce(_pointinit);
    //const Point &translation = -pixmap->mapFromGlobal(Point(0, 0));

    WDebug(debugSquare, "call");

    middle = Point(translation.x(),
                    translation.y() - 50);

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
                    QString("It was not possibile to determinate %1").arg(QString::number((int)action)),
                    log_ui::error_internal);
            WCommonScript::abortIfDebug(__FUNCTION__, __LINE__);
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

    this->needRefreshPrivate();
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
        WCommonScript::swap(topLeft.rx(), bottomRight.rx());

    if(topLeft.y() > bottomRight.y())
        WCommonScript::swap(topLeft.ry(), bottomRight.ry());

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
    if constexpr (WCommonScript::debug_enable()){
        if(un(!painter.isActive())){
            qDebug() << "Painter not active in Square" << __FUNCTION__;
            std::abort();
        }
    }

    if (_need_reload) {
        const auto zoom = doc.getZoom();
        WDebug(debugSquare, "__need_reload = true");

        if(likely(somethingInBox())){
            const PointF point = doc.getPointFirstPage() + _trans_img * zoom;
            const int len = doc.lengthPage();
            const QSize size = createSizeRect(doc, len, DRAW_CREATE_SIZE_RECT_DEF_PRO);

            W_ASSERT(size.height() >= 0 and size.width() >= 0);
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
    _pointinit.setSet(true);

    /** we don't need yet to draw somethings */
    _need_reload = false;
    _in_box = false;

    this->_hideProperty();
}
