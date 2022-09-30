#include "square.h"
#include <QPen>
#include <QPainter>
#include "currenttitle/document.h"
#include <QDebug>
#include "images/fromimage.h"
#include "log/log_ui/log_ui.h"
#include "touch/property/property_control.h"
#include "touch/tabletcanvas.h"
#include "utils/common_script.h"
#include "touch/multi_thread_data.h"
#include "touch/paintevent/paint.h"
#include "pthread.h"
#include "testing/memtest.h"

static pthread_mutex_t      __mutex_sq;
static const page           *__page;
static QPointF              __f;
static QPointF              __s;
static QVector<int>         *__index;
static bool                 *__in_box;

square::square(QObject *parent, property_control *property):
    QObject(parent)
{
    W_ASSERT(parent);
    W_ASSERT(property);
    W_ASSERT(parent->objectName() == "TabletCanvas");

    _thread        = get_thread_max();
    _dataThread    = get_data_max();
    _threadCount   = get_thread_used();

    _property = property;
    WNew(_copy, copy, ());
    _canvas = (TabletCanvas *) parent;

    QObject::connect(_property, &property_control::ActionSelection, this, &square::actionProperty);

    _penna.setStyle(Qt::DotLine);
    _penna.setWidth(2);
    _penna.setColor(QColor::fromRgb(30, 90, 255));
    this->reset();

    __in_box = &_in_box;

    pthread_mutex_init(&__mutex_sq, NULL);
}

square::~square()
{
    free_thread_data(&_thread, &_dataThread);
    WDelete(_copy);
}

void * __square_search(void *__data)
{
    W_ASSERT(__data);
    DataPrivateMuThread *data = (DataPrivateMuThread *)__data;
    bool in_box = false;

    assert(data->from <= data->to);

    for(; data->from < data->to;  data->from ++){
        const Stroke &stroke = __page->atStroke(data->from);

        if(datastruct_isinside(__f, __s, stroke))
        {
            pthread_mutex_lock(&__mutex_sq);
            __index->append(data->from);
            pthread_mutex_unlock(&__mutex_sq);

            in_box = true;
        }
    }

    if(in_box)
        *__in_box = true;

    return NULL;
}

/* 
 * la funzione capisce se all'interno del quadrato
 * della selezione c'è qualcosa in caso salva l'id
 * del tratto e setta la variabile this->check =
 * true, in caso contrario la setta = false e fa il return
*/
bool square::find()
{

    Document *doc = _canvas->data;
    datastruct *data = doc->datatouch;
    bool tmp_find;
    int i, create, lenPage, count;
    int PageCounter;
    QList<QVector<int>> index;

    this->adjustPoint();

    const QPointF &topLeft = _pointinit;
    const QPointF &bottomRight = _pointfine;

#define CTRL_POINT(point) W_ASSERT(point.x() >= 0. && point.y() >= 0.);
    CTRL_POINT(topLeft);
    CTRL_POINT(bottomRight);

    WDebug(debugSquare, "call");

    _base = data->getFirstPageVisible();
    lenPage = data->lengthPage();
    _in_box = false;
    __need_reload = false;
    PageCounter = _base;

    __f     = topLeft;
    __s     = bottomRight;

    /* point selected by user */
    for(count = 0; PageCounter < lenPage; PageCounter ++, count ++){
        __page = &data->at(PageCounter);
        create = DataPrivateMuThreadInit(_dataThread, nullptr, _threadCount, __page->lengthStroke(), 0);

        if(unlikely(!__page->isVisible()))
            break;

        if(likely(count > index.length() - 1))
            index.append(QVector<int>());

        __index = &index.operator[](count);

        for(i = 0; i < create; i++){
            pthread_create(&_thread[i], NULL, __square_search, (void *)&_dataThread[i]);
        }
        for(i = 0; i < create; i++){
            pthread_join(_thread[i], NULL);
        }
    }

    /* image selected by user */
    const int lenImg = doc->m_img->m_img.length();
    for(int counterImg = 0; counterImg < lenImg; counterImg++){
        const auto &ref = doc->m_img->m_img.at(counterImg);

        tmp_find = datastruct_isinside(topLeft, bottomRight, ref.i) ||
                   datastruct_isinside(topLeft, bottomRight, ref.f);

        if(!tmp_find)
            continue;

        _index_img.append(counterImg);
        _in_box = true;
    }

    findObjectToDraw(index);

    moveObjectIntoPrivate(index);

    if(!somethingInBox()){
        if(!_copy->isEmpty()){
            _pointinit = __f;

            _pointinit.setSet(false);
            _pointfine.setSet(false);
        }

        reset();
    }else{
        __need_reload = true;
    }

    return _in_box;
}

force_inline void square::initImg()
{
    const auto l = _canvas->data->datatouch->lengthPage();
    _img.initAsPage(l);
}

void square::mergeImg(
        const WImage    &from,
        WImage          &to,
        int             page)
{
    QPainter painter;
    QRect __to = from.rect();

    __to.translate(0, page * page::getResolutionHeigth());

    painter.begin(&to);
    W_ASSERT(painter.isActive());
    painter.drawImage(__to, from, from.rect());
    painter.end();
}

static force_inline void
preappend(QList<QList<Stroke>> & list, int num)
{
    while(num > 0){
        num --;
        list.append(QList<Stroke>());
    }
}

void square::moveObjectIntoPrivate(QList<QVector<int>> &index)
{
    int count;
    cint len = index.length();
    page * page;
    datastruct & data = *_canvas->data->datatouch;
    WImage tmp;

    WDebug(debugSquare, "call");

    _stroke.clear();

    order_multiple(index);

    this->initImg();

    preappend(_stroke, len);

    for(count = 0; count < len; count ++){
        const QVector<int> & ref = index.at(count);
        WDebug(debugSquare, ref);
        page = &data.at_mod(count + _base);

        if(unlikely(ref.isEmpty()))
            continue;

        page->drawToImage(ref, tmp, DR_IMG_INIT_IMG);

        this->mergeImg(tmp, _img, count + _base);

        page->swap(_stroke.operator[](count), ref, PAGE_SWAP_TRIGGER_VIEW);
    }
}

void square::findObjectToDrawImg()
{
    for(int index : qAsConst(_index_img)){
        const auto &ref = _canvas->data->m_img->m_img.at(index);

        if(ref.i.x() < _pointinit.x())
            _pointinit.setX(ref.i.x());
        else if(ref.f.x() > _pointfine.x())
            _pointfine.setX(ref.f.x());

        if(ref.i.y() < _pointinit.y())
            _pointinit.setY(ref.i.y());
        else if(ref.f.y() > _pointfine.y())
            _pointfine.setY(ref.f.y());
    }
}

/*
 * la funzione viene richiamata quando dobbiamo
 * spostare un po' di oggetti nella lista m_id
 * in questo caso si analizza quando c'è un id
 *  uguale, e si sposta tutto il tratto
*/
void square::findObjectToDraw(const QList<QVector<int>> &index)
{
    const datastruct *data = _canvas->data->datatouch;
    const auto trans = data->getPointFirstPage();
    QRectF sizeData;
    return;
    WDebug(debugSquare, "call");

    if(unlikely(index.isEmpty()))
        goto img;

    // find the first point
    sizeData = data->get_size_area(index, _base);

    _pointinit = sizeData.topLeft() + trans;
    _pointfine = sizeData.bottomRight() + trans;

img:
    findObjectToDrawImg();
}

void square::reset()
{
    int i, len;
    WDebug(debugSquare, "call");
    _pointinit.setSet(false);
    _lastpoint.setSet(false);
    _pointfine.setSet(false);

    _in_box = false;

    __need_reload = false;
    _index_img.clear();

    WDebug(debugSquare, "paste = 1");
    len = _stroke.length();
    if(len == 0)
        goto out;

    for(i = 0; i < len; i++){
        QList<Stroke> ll    = _stroke.operator[](i);
        page * page         = &_canvas->data->datatouch->at_mod(i + _base);

        for(auto &ref : ll){
            ref.scale(_trans_img);
        }

        page->append(ll);
        page->triggerRenderImage(-1, false);
    }

    _stroke.clear();

out:
    this->_img = WImage();
    this->_stroke.clear();
    this->_trans_img = QPointF(0.0, 0.0);
}

void square::initPointMove(const QPointF &point)
{
    QPointF new_point;
    datastruct *Data = _canvas->data->datatouch;
    QRectF rect(_pointinit, _pointfine);
    WDebug(debugSquare, "call");

    new_point = Data->adjustPoint(point);

    _lastpoint = PointSettable(point, true);

    WDebug(debugSquare, "initPointMove" << rect.topLeft() << rect.bottomRight() << new_point);

    if(!rect.contains(new_point)){
        WDebug(debugSquare, "Not in box");
        this->reset();
        this->initPoint(point);
    }
}

void square::move(const QPointF &punto)
{
    QPointF delta;
    Document *data = _canvas->data;
    const auto zoom = data->datatouch->getZoom();


    WDebug(debugSquare, "call");

#ifdef DEBUGINFO
    W_ASSERT(somethingInBox());
#else
    if(unlikely(!somethingInBox())){
        NAME_LOG_EXT->write("somethingInBox", log_ui::possible_bug);
        this->changeInstrument();
        return;
    }
#endif

    delta = (_lastpoint - punto) / zoom;

    datastruct::inverso(delta);

    _trans_img += delta;

    data->m_img->moveImage(_index_img, delta);

    _pointinit += delta;
    _pointfine += delta;

    _lastpoint = punto;
    __need_reload = true;
}

void square::endMoving(const QWidget *pixmap)
{
    QPoint middle;
    const auto ref = _canvas->data->datatouch->getPointFirstPage();
    int flag;
    const QPoint &translation = -pixmap->mapFromGlobal(QPoint(0, 0));

    WDebug(debugSquare, "call");

    middle = QPoint(_pointinit.x() + translation.x() + ref.x(),
                    _pointinit.y() + translation.y() + ref.y() - _property->height());

    flag = this->calculate_flags();

    WDebug(debugSquare, flag << middle );

    this->_property->Show(middle, flag);
}

void square::actionProperty(property_control::ActionProperty action)
{
    int flags = 0, dontcall_copy = 1;
    datastruct &data = *_canvas->data->datatouch;

    switch (action) {
        case property_control::ActionProperty::__copy:{
            flags = SELECTION_FLAGS_COPY;
            break;
        }
        case property_control::ActionProperty::__cut:{
            flags = SELECTION_FLAGS_CUT;
            break;
        }
        case property_control::ActionProperty::__paste:{
            flags = SELECTION_FLAGS_PASTE;
            break;
        }
        case property_control::ActionProperty::__delete:{
            _stroke.clear();

            //data.removePointIndex(index, base, true);
            dontcall_copy = 0;
            _property->Hide();
            _stroke.clear();
            this->reset();
            break;
        }
        default:{
            NAME_LOG_EXT->write(
                        QString("It was not possibile to determinate %1").arg(QString::number((int)action)),
                        log_ui::error_internal);
            abortIfDebug(__FUNCTION__, __LINE__);
        }
    }

    if(dontcall_copy){
        if(_copy->selection(data, _stroke, flags, _pointinit)){
            _stroke.clear();
            _property->Hide();
            this->reset();
        }
    }
    else{
        this->reset();
    }

    _canvas->call_update();
}

/*
 * la funcione cambia i punti, in caso l'utente non abbia tracciato il
 * rettangolo da in alto a sinistra a in alto a destra
*/
force_inline void square::adjustPoint()
{
    QPointF &topLeft        = _pointinit;
    QPointF &bottomRight    = _pointfine;

    WDebug(debugSquare, topLeft << bottomRight);

    if(topLeft.x() > bottomRight.x())
        __swap(topLeft.rx(), bottomRight.rx());

    if(topLeft.y() > bottomRight.y())
        __swap(topLeft.ry(), bottomRight.ry());

    WDebug(debugSquare, topLeft << bottomRight);

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());
}

static void square_draw_square(
        QPainter            &painter,
        const datastruct    *data,
        const QPointF       &tl,
        const QPointF       &br)
{
    const QPointF TL = data->adjustPointReverce(tl);
    const QPointF BR = data->adjustPointReverce(br);
    constexpr const auto debugDraw = true;

    WDebug(debugSquare && debugDraw, tl << br << TL << BR);

    painter.drawRect(QRectF(TL, BR));
}

void square::needReload(QPainter &painter)
{

    if(debug_enable()){
        if(unlikely(!painter.isActive())){
            qDebug() << "Painter not active in square" << __FUNCTION__;
            std::abort();
        }
    }

    if(__need_reload){
        const auto *data = _canvas->data->datatouch;
        const auto zoom = data->getZoom();
        WDebug(debugSquare, "__need_reload = true");

        if(likely(somethingInBox())){
            const QPointF point = data->getPointFirstPage() + _trans_img * zoom;
            const int len = data->lengthPage();
            const QSize size = createSizeRect(data, len, DRAW_CREATE_SIZE_RECT_DEF_PRO);

            W_ASSERT(size.height() >= 0 && size.width() >= 0);
            WDebug(debugSquare, "in_box");

            singleLoad(painter, _img, size, point,
                       0, DRAW_SINGLE_LOAD_DEF);
        }

        painter.setPen(_penna);
        square_draw_square(painter, data, _pointinit, _pointfine);
    }
}

void square::updatePoint(const QPointF &puntofine)
{
    const datastruct *data = _canvas->data->datatouch;
    WDebug(debugSquare, "call");
    W_ASSERT(!somethingInBox());

    _pointfine = data->adjustPoint(puntofine);

    W_ASSERT(_pointfine.x() >= 0.0 && _pointinit.y() >= 0.0);

    _pointfine = true;

    __need_reload = true;
}

void square::initPoint(const QPointF &point)
{
    const datastruct *data = _canvas->data->datatouch;
    WDebug(debugSquare, "call");

    W_ASSERT(!somethingInBox());

    _pointinit = data->adjustPoint(point);
    W_ASSERT(_pointinit.x() >= 0.0 && _pointinit.y() >= 0.0);
    _pointinit = true;

    /* we don't need yet to draw somethings */
    __need_reload = false;
    _in_box = false;

    this->_property->Hide();
}
