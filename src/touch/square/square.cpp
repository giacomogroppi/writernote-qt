#include "square.h"
#include <QPen>
#include <QPainter>
#include "currenttitle/document.h"
#include <QDebug>
#include "images/fromimage.h"
#include "touch/property/property_control.h"
#include "touch/tabletcanvas.h"
#include "utils/common_script.h"
#include "touch/multi_thread_data.h"
#include "touch/paintevent/paint.h"
#include "pthread.h"

#define SQ_THREAD 8

#ifdef DEBUG_THREAD
# undef SQ_THREAD
# define SQ_THREAD DEBUG_THREAD
#endif

void * __square_search(void *__data);
static pthread_mutex_t      __mutex_sq;
static const page           *__page;
static QPointF              __f;
static QPointF              __s;
static QVector<int>         *__index;
static bool                 *__in_box;

square::square(QObject *parent, property_control *property):
    QObject(parent)
{
    Q_ASSERT(parent);
    Q_ASSERT(property);

    this->m_property = property;
    this->m_copy = new copy();
    this->canvas = (TabletCanvas *) parent;

    QObject::connect(m_property, &property_control::ActionSelection, this, &square::actionProperty);

    penna.setStyle(Qt::DotLine);
    penna.setWidth(2);
    penna.setColor(QColor::fromRgb(30, 90, 255));
    this->reset(false);

    __in_box = &in_box;

    pthread_mutex_init(&__mutex_sq, NULL);
}

square::~square()
{
    delete this->m_copy;
}

/* 
 * la funzione capisce se all'interno del quadrato
 * della selezione c'è qualcosa in caso salva l'id
 * del tratto e setta la variabile this->check =
 * true, in caso contrario la setta = false e fa il return
*/

bool square::find(){
    Document *doc = canvas->data;
    datastruct *data = doc->datatouch;
    bool tmp_find;
    int i, create, lenPage, count;
    const QPointF &translation = data->getPointFirstPage();
    int PageCounter;

    QList<QVector<int>> index;

    const QPointF &topLeft = data->adjustPoint(pointinit.point);
    const QPointF &bottomRight = data->adjustPoint(pointfine.point);

    pthread_t thread[SQ_THREAD];
    DataPrivateMuThread dataThread[SQ_THREAD];

    base = data->getFirstPageVisible();
    lenPage = data->lengthPage();
    in_box = false;
    __need_reload = false;
    PageCounter = base;

    __f     = topLeft + translation;
    __s     = bottomRight + translation;

    this->adjustPoint();

    W_ASSERT(pointinit.x() <= pointfine.x());
    W_ASSERT(pointinit.y() <= pointfine.y());

    /*
     * Il problema di cercare nella lista è
     * il tempo sia di scorrerla, sia di creare
     * l'area degli stroke. Siccome ogni stroke
     * è (thread save) l'uno rispetto all'altro lo
     * possiamo fare in parallelo.
     * Ci salviamo la posizione degli stroke
     * in lista e settiamo le pagine interessate
     * per non poter modificare la lista.
     */

    /* point selected by user */
    for(count = 0; PageCounter < lenPage; PageCounter ++, count ++){
        __page = &data->at(PageCounter);
        create = DataPrivateMuThreadInit(dataThread, NULL, SQ_THREAD, __page->lengthStroke());

        if(unlikely(!__page->isVisible()))
            break;

        if(likely(count > index.length() - 1))
            index.append(QVector<int>());

        __index = &index.operator[](count);

        for(i = 0; i < create; i++){
            pthread_create(&thread[i], NULL, __square_search, (void *)&dataThread[i]);
        }
        for(i = 0; i < create; i++){
            pthread_join(thread[i], NULL);
        }
    }

    /* image selected by user */
    const int lenImg = doc->m_img->m_img.length();
    for(int counterImg = 0; counterImg < lenImg; counterImg++){
        const auto &ref = doc->m_img->m_img.at(counterImg);

        tmp_find = datastruct::isinside(topLeft, bottomRight, ref.i) ||
                   datastruct::isinside(topLeft, bottomRight, ref.f);

        if(!tmp_find)
            continue;

        this->m_index_img.append(counterImg);
        this->in_box = true;
    }

    findObjectToDraw(index);

    moveObjectIntoPrivate(index);

    if(!somethingInBox()){
        reset(false);
    }else{
        __need_reload = true;
    }

    return in_box;
}

void square::initImg()
{
    const auto formact = QImage::Format_ARGB32;
    img = QImage(page::getResolutionWidth(),
                 page::getResolutionHeigth() * canvas->data->datatouch->lengthPage(),
                 formact);
}

void square::mergeImg(
        const QImage    &from,
        QImage          &to,
        int             page)
{
    QPainter painter;
    QRect __to = from.rect();

    __to.translate(0, page * page::getResolutionHeigth());

    painter.begin(&to);
    painter.drawImage(__to, from, from.rect());
    painter.end();
}

static void preappend(QList<QList<stroke>> & list, int num)
{
    while(num > 0){
        num --;
        list.append(QList<stroke>());
    }
}

void square::moveObjectIntoPrivate(QList<QVector<int>> &index)
{
    int count;
    cint len = index.length();
    page * page;
    datastruct & data = *canvas->data->datatouch;
    QImage tmp;

    DO_IF_DEBUG(const auto m_index_tmp = index);

    this->m_stroke.clear();

    order_multiple(index);

    DO_IF_DEBUG_ENABLE(true,
    int i;
    for(i = 0; i < m_index_tmp.length(); i++){
        const auto &ref = index.at(i);
        const auto &tmp = m_index_tmp.at(i);

        if(ref.length() != tmp.length())
            std::abort();

        for(int c = 0; c < ref.length(); c++){
            if(ref.indexOf(tmp.at(i)) == -1)
                std::abort();
        }
    }
    );

    this->initImg();

    preappend(m_stroke, len);

    for(count = 0; count < len; count ++){
        const QVector<int> & ref = index.at(count);
        qDebug() << ref;
        page = &data.at_mod(count + base);

        page->drawToImage(ref, tmp, DR_IMG_INIT_IMG);

        this->mergeImg(tmp, img, count + base);

        page->swap(m_stroke.operator[](count), ref);
    }
}

/* la funzione resistuisce
 * vero se è intero il punto è interno
*/
bool square::isinside(const QPointF &point){
    return datastruct::isinside(pointinit.point, pointfine.point, point);
}

/*
 * la funzione viene richiamata quando dobbiamo
 * spostare un po' di oggetti nella lista m_id
 * in questo caso si analizza quando c'è un id
 *  uguale, e si sposta tutto il tratto
*/
void square::findObjectToDraw(const QList<QVector<int>> &index)
{
    const datastruct *data = canvas->data->datatouch;
    QRectF sizeData;

    if(unlikely(index.isEmpty()))
        goto img;

    // find the first point
    sizeData = data->get_size_area(index, this->base);

    this->pointinit.point = sizeData.topLeft();
    this->pointfine.point = sizeData.bottomRight();

img:
    for(int counterImg = 0; counterImg < m_index_img.length(); counterImg ++){
        const int index = this->m_index_img.at(counterImg);
        const auto &ref = canvas->data->m_img->m_img.at(index);

        if(ref.i.x() < pointinit.point.x())
            pointinit.point.setX(ref.i.x());
        else if(ref.f.x() > pointfine.point.x())
            pointfine.point.setX(ref.f.x());

        if(ref.i.y() < pointinit.point.y())
            pointinit.point.setY(ref.i.y());
        else if(ref.f.y() > pointfine.point.y())
            pointfine.point.setY(ref.f.y());
    }
}

void square::reset(bool paste)
{
    pointinit.set = lastpoint.set = pointfine.set = false;

    in_box = false;

    __need_reload = false;
    m_index_img.clear();

    if(likely(paste)){
        for(int i = 0; i < m_stroke.length(); i++){
            QList<stroke> ll    = m_stroke.operator[](i);
            page * page         = &canvas->data->datatouch->at_mod(i + base);

            for(auto &ref : ll){
                ref.scale(this->trans_img, STROKE_MUST_TRASLATE_PATH);
            }

            page->append(ll);
            page->triggerRenderImage(-1, false);
        }
    }

    this->img = QImage();
    this->m_stroke.clear();
    this->trans_img = QPointF(0.0, 0.0);
}

void square::move(const QPointF &punto){
    QPointF delta;
    Document *data = canvas->data;
    QList<int> PageModify;

#ifdef DEBUGINFO
    W_ASSERT(somethingInBox());
#else
    if(unlikely(somethingInBox())){
        NAME_LOG_EXT->write("somethingInBox", log_ui::possible_bug);
        this->changeInstrument(false);
        return;
    }
#endif

    if(unlikely(lastpoint.isNotSet())){
        lastpoint = PointSettable(punto, true);
        return;
    }

    if(!datastruct::isinside(pointinit.point, pointfine.point, punto)){
        return this->reset(true);
    }

    delta = lastpoint.point - punto;

    datastruct::inverso(delta);

    for(QList<stroke> & tmp : m_stroke){
        datastruct::MovePoint(tmp, delta, 0);
    }

    this->trans_img += delta;
    this->img.scaled(delta.x(), delta.y());

    data->m_img->moveImage(m_index_img, delta);

    lastpoint.point = punto;

    pointinit.point += delta;
    pointfine.point += delta;

    __need_reload = true;
    data->datatouch->triggerNewView(PageModify, -1, true);
}

void square::endMoving(const QWidget *pixmap)
{
    QPoint middle;
    int flag;
    const QPoint &translation = -pixmap->mapFromGlobal(QPoint(0, 0));

    middle = QPoint(pointinit.x() + translation.x(),
                    pointinit.y() + translation.y() - m_property->height());

    flag = this->calculate_flags();

    qDebug() << "square::needReload show" << flag << middle;

    this->m_property->Show(middle, flag);
}

void square::actionProperty(property_control::ActionProperty action)
{
    int flags = 0, dontcall_copy = 1;
    QList<int> page;
    datastruct &data = *canvas->data->datatouch;

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
            this->m_stroke.clear();

            //data.removePointIndex(index, base, true);
            dontcall_copy = 0;
            m_property->Hide();
            break;
        }
        default:{
            NAME_LOG_EXT->write(
                        QString("It was not possibile to determinate %1").arg(QString::number((int)action)),
                        log_ui::error_internal);
            abortIfDebug();
        }
    }

    if(dontcall_copy)
        std::abort();//this->m_copy->selection(data, this->index, base,
         //                       flags, page, pointinit.point);
    else
        this->reset(false);

    data.triggerNewView(page, -1, true);

    if(!page.isEmpty())
        canvas->call_update();

    this->canvas->call_update();
}

#define MAKE_CHANGE(point1, point2, function, secFunction)  \
    do{                                                     \
        if(point1.function() > point2.function())           \
        {                                                   \
            tmp = point2.function();                        \
            point2.secFunction(point1.function());          \
            point1.secFunction(tmp);                        \
        }                                                   \
    }while(0)

/*
 * la funcione cambia i punti, in caso l'utente non abbia tracciato il
 * rettangolo da in alto a sinistra a in alto a destra
*/
Q_ALWAYS_INLINE void square::adjustPoint()
{
    double tmp;
    QPointF &topLeft = pointinit.point;
    QPointF &bottomRight = pointfine.point;

    MAKE_CHANGE(topLeft, bottomRight, x, setX);
    MAKE_CHANGE(topLeft, bottomRight, y, setY);
}

void * __square_search(void *__data)
{
    DataPrivateMuThread *data = (DataPrivateMuThread *)__data;
    bool in_box = false;

    for(; data->from < data->to;  data->from ++){
        const stroke &stroke = __page->atStroke(data->from);
        if(datastruct::isinside(__f, __s, stroke)){

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

void square::needReload(QPainter &painter)
{
    QPointF point;
    datastruct *data;
    int len;

    if(likely(__need_reload)){
        if(in_box){
            data = canvas->data->datatouch;
            point = data->getPointFirstPage();
            len = data->lengthPage();

            singleLoad(painter, this->img, createSizeRect(data, len, DRAW_CREATE_SIZE_RECT_DEF_PRO),
                       point + this->trans_img, 0, DRAW_SINGLE_LOAD_DEF);
        }

        painter.setPen(this->penna);
        painter.drawRect(QRectF(pointinit.point, pointfine.point));
    }
}
