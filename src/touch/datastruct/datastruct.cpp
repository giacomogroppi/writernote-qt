#include "datastruct.h"
#include <QList>
#include <QDebug>
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
    this->_zoom = zoom;
    if(canvas){
        canvas->callResizeEvent();
        canvas->_parent->zoomChange();
    }
}

void datastruct::increaseZoom(const double delta, const QSize &size)
{
    this->_zoom += delta;
    this->adjustAll(size);
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

void datastruct::reset(){
    this->_page.clear();
    _pointFirstPage = QPointF(0, 0);
    _zoom = 1.00;
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
        diff = -diff;
        for(i = 0; i < diff; i++)
            dest.removePage(i);
    }

    for(i=0; i<len; ++i){
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
 * ritorna false se anche con uno spostamento non siamo
 * riusciti a coprire la parte di pixmap fuori dal
 * foglio
*/
void datastruct::adjustHeight(const uint height)
{
    const QPointF point = this->getPointFirstPage();
    QPointF t(0.0, 0.0);

    double y = biggery();

    if(y < height){
        t.setX(height-y);
    }else{ //(x >= width)
        if(point.y() > 0.0)
            t.setX(-point.y());
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
void datastruct::adjustWidth(const uint width)
{
    QPointF point = this->getPointFirstPage();
    QPointF t(0., 0.);
    double biggerX = biggerx();

    if(point.x() < 0. && biggerX <= width){
        t.setX((double)width - biggerX);
    }else{ //(x >= width)
        if(point.x() > 0.)
            t.setX(- point.x());
    }

    scala_all(t * _zoom);

    point = this->getPointFirstPage();
    if(unlikely(point.x() > 0.)){
        this->setPointFirstPage(QPointF(0.0, point.y()));
    }
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

    QPointF point = this->getPointFirstPage();
    qDebug() << "datastruct::repositioning" << point;
    this->_zoom = 1.0;

    datastruct::inverso(point);

    scala_all(point, INT_MAX);
}

void datastruct::restoreLastTranslation(const int heightView){
    if(_last_translation == QPointF(0, 0))
        return;

    datastruct::inverso(_last_translation);

    scala_all(_last_translation, heightView);

    _last_translation = QPointF(0, 0);
}

void datastruct::scala_all(const QPointF &point, const int heightView)
{
    if(unlikely(point == QPointF(0, 0)))
        return;

    this->_pointFirstPage += point;
    this->pageVisible = -1;

#ifdef PDFSUPPORT
    _pdf->translation(point);
#endif

    _img->move(point);

    if(likely(heightView > 0)){
        this->triggerVisibility(heightView);
    }
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottonright, const stroke &stroke)
{
    return stroke.isInside(QRectF(topleft, bottonright));
}

bool datastruct::isinside(const QPointF &topleft, const QPointF &bottomright, const uint IndexPage, const uint IndexStroke) const
{
    const stroke &stroke = at(IndexPage).atStroke(IndexStroke);
    return datastruct::isinside(topleft, bottomright, stroke);
}

/* the list can be not order */
void datastruct::MovePoint(
        const QVector<int>  &pos,
        cint                __page,
        const QPointF       &translation)
{
    page &page = at_mod(__page);
    for(const auto &index : qAsConst(pos)){
        stroke & stroke = page.atStrokeMod(index);
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
        if(!is_order(pos)){
            std::abort();
        }
#else
        if(unlikely(is_order(pos))){
            NAME_LOG_EXT->write("List not order", log_ui::critic_error);
            /* in this case we need to order */
            order(pos);
        }

#endif
    }else{
        order(pos);
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

void datastruct::removePage(uint page)
{
    uint i;
    this->_page.removeAt(page);

    for(i = 0; i < page; i++){
        _page.operator[](i)._count = i + 1;
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

/*
 * return true if the user has written something
*/
bool datastruct::userWrittenSomething(datastruct *s_data)
{
    int l_first_page, l_sec_page, counterPage, counterStroke;
    bool check = false;

    l_first_page = lengthPage();
    if(s_data)
        l_sec_page = s_data->lengthPage();

    if(!s_data){
        counterPage = 0;
        goto single;
    }

    for(counterPage = 0; counterPage < l_first_page && counterPage < l_sec_page; counterPage++){

        const page &page1 = this->at(counterPage);
        const page &page2 = s_data->at(counterPage);

        for(counterStroke = 0; counterStroke < page1.lengthStroke() && counterStroke < page2.lengthStroke(); counterStroke ++){
            const stroke &firstStroke  = page2.atStroke(counterStroke);
            const stroke &secondStroke = page1.atStroke(counterStroke);

            if(firstStroke.is_complex()){
                check = stroke_complex_cmp(&firstStroke, &secondStroke);
                if(!check)
                    goto ret;
                continue;
            }

            int firstLen, secondLen;
            firstLen = firstStroke.length();
            secondLen = secondStroke.length();

            for(int i = 0; i < firstLen && i < secondLen; i++){
                const point_s &firstPoint = firstStroke.at(i);
                const point_s &secondPoint = secondStroke.at(i);

                if(firstPoint != secondPoint){
                    check = false;
                    goto ret;
                }
            }

            if(firstStroke.length() < secondStroke.length()){
                check = false;
                goto ret;
            }

        }

        if(page1.lengthStroke() < page2.lengthStroke()){
            check = false;
            goto ret;
        }
    }

    if(l_first_page == l_sec_page){
        check = false;
        goto ret;
    }

    if((counterPage < l_sec_page) && s_data){
        check = s_data->userWrittenSomething(counterPage);
    }

single:
    if(counterPage < l_first_page){
        check = userWrittenSomething(counterPage);
    }

ret:
    return check;
}
