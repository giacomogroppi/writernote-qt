#include "datastruct.h"
#include <QList>
#include <QDebug>
#include "core/core.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "touch/tabletcanvas.h"
#include "mainwindow.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

#define mov_if_neg(p, x) \
    p = x; \
    return;

void datastruct::newPage(int num)
{
    while (num > 0){
        this->newPage(n_style::white);
        num --;
    }
}

void datastruct::changeZoom(const double zoom, TabletCanvas *canvas)
{
    const double size = canvas->size().height();

    if(unlikely(!datastruct::isOkZoom(zoom))){
        return;
    }

    this->_zoom = zoom;

    if(likely(canvas)){
        canvas->callResizeEvent();
        core::get_main_window()->zoomChange();
    }

    this->triggerVisibility(size);
    this->pageVisible = -1;
    (void)this->getFirstPageVisible();
}

void datastruct::increaseZoom(const double delta, const QSize &size)
{
    this->_zoom += delta;
    this->adjustAll(size);
    this->triggerVisibility(size.height());
    this->pageVisible = -1;
}

void datastruct::drawIfInside(const QRect &area)
{
    int i = this->getFirstPageVisible();

    for(; i >= 0; i--){
        page *page = &at_mod(i);

        if(!page->isVisible())
            break;

        page->drawIfInside(-1, area);
    }
}

datastruct::datastruct(frompdf *m_pdf, fromimage *m_img)
{
    this->_pdf = m_pdf;
    this->_img = m_img;
    _last_translation = QPointF(0, 0);

    pthread_mutex_init(&_changeIdMutex, NULL);
    pthread_mutex_init(&_changeAudioMutex, NULL);
}

datastruct::~datastruct()
{
    pthread_mutex_destroy(&_changeIdMutex);
    pthread_mutex_destroy(&_changeAudioMutex);
}

void datastruct::triggerIfNone(int m_pos_ris)
{
    int i = lengthPage();
    for(i --; i >= 0; i--){
        page &_page = at_mod(i);
        if(_page._imgDraw.isNull()){
            _page.triggerRenderImage(m_pos_ris, true);
        }
    }
}

void datastruct::reset(){
    this->_page.clear();
#ifdef PDFSUPPORT
    this->_pdf->reset();
#endif
    this->_img->reset();
    this->pageVisible = -1;
    _pointFirstPage = QPointF(0, 0);
    _zoom = 1.;
}

void datastruct::decreaseAlfa(const QVector<int> &pos, int index)
{
    at_mod(index).decreseAlfa(pos, 4);
}

void datastruct::copy(const datastruct &src, datastruct &dest)
{
    int i;
    const int len = src.lengthPage();
    int diff = len - dest.lengthPage();

    if(diff > 0){
        for(i = 0; i < diff; i++){
            dest.newPage(n_style::white);
        }
    }
    else if(diff != 0){
        i = - diff - 1;

        for(; i >= 0; i--){
            dest.removePage(i);
        }
    }

    for(i = 0; i < len; i++){
        page::copy(src._page.at(i), dest._page.operator[](i));
    }

    dest._zoom = src._zoom;
    dest.pageVisible = src.pageVisible;
    dest._pointFirstPage = src._pointFirstPage;

    dest._last_translation = src._last_translation;
}

/*
 * la funzione controlla che non si sia delle parti
 * del pixmap fuori dal foglio, in caso scala tutto
 * di quanto basta.
 *
*/
void datastruct::adjustHeight(cdouble height)
{
    const QPointF point = this->getPointFirstPageNoZoom();
    QPointF t(0.0, 0.0);
    uchar not_used ff = 0;
    double y = biggery();

    if(point.y() > 0.0){
        ff = 1;
        t.setY( - point.y());
        qDebug() << "Need to restore first Point" << point;
    }
    else if(y < height){
        ff = 2;
        t.setY((height - y) / _zoom);

        if(!isOkTranslate(t, true)){
            t.setY(0.);
        }
    }

    scala_all(t, height);
}

/*
 * la funzione viene chiamata quando si zoom diminuendo lo zoom
 * per evitare problemi si controlla che il pixel più grosso all'interno della
 * pagina sia fuori dal pixmap, in larghezza, in caso sia dentro, si verifica se
 * è possibile spostare tutti i dati a destra, se si si sposta e si rifà il
 * controllo che siano fuori, in caso contrario si fa il return di false e
 * bisogna rifare il pixmap
*/
void datastruct::adjustWidth(cdouble width)
{
    const QPointF point = this->getPointFirstPage();
    QPointF t(0., 0.);
    double biggerX = biggerx();
    bool not_used __f;
    cdouble x = point.x();

    if(x < 0. && biggerX <= width){
        t.setX(width - biggerX);
        __f = true;
        if(!isOkTranslate(t, true)){
            t.setX(0.);
        }

    }else{ //(x >= width)
        if(point.x() > 0.){
            __f = false;
            t.setX(-point.x());
        }
    }

    scala_all(t / _zoom);
}

/*
 * la funzione ritorna adjustWidth
 * the function consider the fact that the
 * height of one sheet is bigger than the width
*/
void datastruct::adjustAll(const uint width,
                           const uint height)
{
    adjustWidth(width);
    adjustHeight(height);
}

void datastruct::adjustAll(const QSize &size)
{
    this->adjustAll(size.width(), size.height());
}

/*
 * return true if the first point is different
 * from (0.0, 0.0)
*/
void datastruct::repositioning()
{
    if(isempty())
        return;

    QPointF point = this->getPointFirstPageNoZoom();
    qDebug() << "datastruct::repositioning" << point;
    this->_zoom = 1.0;

    datastruct::inverso(point);

    scala_all(point, INT_MAX);
}

void datastruct::restoreLastTranslation(const int heightView)
{
    if(_last_translation == QPointF(0, 0))
        return;

    datastruct::inverso(_last_translation);

    scala_all(_last_translation, heightView);

    _last_translation = QPointF(0, 0);
}

void datastruct::scala_all(const QPointF &point, const int heightView)
{
    constexpr double prec = .00005;
    if(unlikely(point == QPointF(0, 0)))
        return;

    W_ASSERT(_pointFirstPage.x() + point.x() - prec <= 0.);
    W_ASSERT(_pointFirstPage.y() + point.y() - prec <= 0.);

    this->_pointFirstPage += point;
    this->pageVisible = -1;

#ifdef PDFSUPPORT
    _pdf->translation(point);
#endif

    _img->move(point);

    if(likely(heightView > 0)){
        this->triggerVisibility(heightView);
    }else{
        cint range = this->get_range_visible();
        int first = this->getFirstPageVisible();

        if(unlikely(first) < 0)
            first = 0;

        this->setVisible(first, first + range);
    }

    W_ASSERT(this->getPointFirstPageNoZoom().x() <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() <= 0.);
}

/* the list can be not order */
void datastruct::MovePoint(
        const QVector<int>  &pos,
        cint                __page,
        const QPointF       &translation)
{
    page &page = at_mod(__page);
    for(const auto &index : qAsConst(pos)){
        Stroke & stroke = page.atStrokeMod(index);
        stroke.movePoint(translation);
    }
}

void datastruct::removePointIndex(
        QVector<int>    &pos,
        cint            __page,
        cbool           __isOrder)
{
    page *page = &at_mod(__page);
    int i = pos.length() - 1;

    if(likely(__isOrder)){
#ifdef DEBUGINFO
        W_ASSERT(is_order_vector(pos));
#else
        if(unlikely(is_order_vector(pos))){
            NAME_LOG_EXT->write("List not order", log_ui::critic_error);
            /* in this case we need to order */
            order_vector(pos);
        }

#endif
    }else{
        order_vector(pos);
    }

    for(; i >= 0; i --){
        page->removeAt(pos.at(i));
    }
}

void datastruct::removePointIndex(
        QList<QVector<int> >    &pos,
        cint                    base,
        cbool                   __isOrder)
{
    int i, lenList;
    lenList = pos.length();

    for(i = 0; i < lenList; i++){
        removePointIndex(pos.operator[](i), base + i, __isOrder);
    }

}

void datastruct::removePage(const int page)
{
    int i, len;
    len = this->lengthPage();

    W_ASSERT(page >= 0 && page < len);

    this->_page.removeAt(page);

    len --;

    for(i = page; i < len; i++){
        auto &_page = at_mod(i);
        _page.setCount(i + 1);
    }

    this->pageVisible = -1;
}

void datastruct::moveToPage(int newPage)
{
    const int range = this->get_range_visible();

    this->setPointFirstPage(QPointF(0., - at(newPage).minHeight()));

    W_ASSERT(this->getPointFirstPageNoZoom().x() <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() <= 0.);

    if(debug_enable()){
        const auto not_used point = this->adjustPoint(QPointF(0., 0.));
        const auto not_used index = this->whichPage(point);
        qDebug() << newPage << index << this->getPointFirstPage() << get_range_visible();
        W_ASSERT(index == newPage);
    }

    this->setVisible(newPage, newPage + range);
}

int datastruct::getLastPageVisible() const
{
    int i = lengthPage();

    for(i --; i >= 0; i--){
        if(at(i).isVisible()){
            W_ASSERT(this->getFirstPageVisible() <= i);
            return i;
        }
    }

    abortIfDebug(__FILE__, __LINE__);
    return -1;
}

void datastruct::insertPage(const page &Page, int index)
{
    int len;

    this->_page.insert(index, Page);

    len = this->lengthPage();

    for(index ++; index < len; index ++){
        this->at_mod(index).setCount(index + 1);
    }
}

bool datastruct::userWrittenSomething(uint frompage)
{
    const uint len = lengthPage();
    for(; frompage < len; frompage++){
        if(at(frompage).userWrittenSomething())
            return true;
    }

    return false;
}

bool datastruct::userWrittenSomething() const
{
    int i;
    int lenPage = this->lengthPage();
    for(i = 0; i < lenPage; i++){
        if(at(i).lengthStroke() != 0)
            return true;
    }

    return false;
}

/*
 * return true if the user has written something
*/
bool datastruct::userWrittenSomething(const datastruct &data1, const datastruct &data2)
{
    int i;
    cint page1 = data1.lengthPage();

    if(page1 != data2.lengthPage())
        return true;

    for(i = 0; i < page1; i++){
        int k;
        const auto &Page1 = data1.at(i);
        const auto &Page2 = data2.at(i);
        cint stroke1 = Page1.lengthStroke();

        if(stroke1 != Page2.lengthStroke()){
            return true;
        }

        for(k = 0; k < stroke1; k++){
            bool check;
            const auto &Stroke1 = Page1.atStroke(k);
            const auto &Stroke2 = Page2.atStroke(k);

            if(Stroke1.is_complex()){
                check = stroke_complex_cmp(&Stroke1, &Stroke2);
                if(!check)
                    return true;
                continue;
            }
            check = Stroke::cmp(Stroke1, Stroke2);
            if(!check)
                return true;
        }
    }

    return false;
}
