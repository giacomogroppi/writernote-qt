#include "datastruct.h"
#include <QList>
#include <QDebug>
#include "core/core.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "touch/tabletcanvas.h"
#include "mainwindow.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"
#include "log/log_ui/log_ui.h"

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

    if(un(!datastruct::isOkZoom(zoom))){
        return;
    }

    this->_zoom = zoom;

    if(likely(canvas)){
        canvas->callResizeEvent();
        core::get_main_window()->zoomChange();
    }

    this->triggerVisibility(size);
    this->_pageVisible = -1;
    (void)this->getFirstPageVisible();
}

void datastruct::increaseZoom(const double delta, const QSize &size, QPointF &res)
{
    this->_zoom += delta;
    this->adjustAll(size, res);
    this->triggerVisibility(size.height());
    this->_pageVisible = -1;
}

void datastruct::drawIfInside(const QRect &area)
{
    int i = this->getFirstPageVisible();

    for(; i >= 0; i--){
        Page *page = &at_mod(i);

        if(!page->isVisible())
            break;

        page->drawIfInside(-1, area);
    }
}

datastruct::datastruct()
{
    _last_translation = QPointF(0., 0.);
    _zoom = 1.;
}

void datastruct::triggerIfNone(int m_pos_ris)
{
    for(auto &page : _page){
        if(page._imgDraw.isNull())
            page.triggerRenderImage(m_pos_ris, true);
    }
}

void datastruct::reset_touch()
{
    _page.clear();
    _pageVisible = -1;
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
        Page::copy(src._page.at(i), dest._page.operator[](i));
    }

    dest._zoom = src._zoom;
    dest._pageVisible = src._pageVisible;
    dest._pointFirstPage = src._pointFirstPage;

    dest._last_translation = src._last_translation;
}

/*
 * la funzione controlla che non si sia delle parti
 * del pixmap fuori dal foglio, in caso scala tutto
 * di quanto basta.
 *
*/
void datastruct::adjustHeight(cdouble height, QPointF& translateTo)
{
    const QPointF point = this->getPointFirstPageNoZoom();
    uchar not_used ff = 0;
    double y = biggery();

    if(point.y() > 0.0){
        ff = 1;
        translateTo.setY( - point.y());
        qDebug() << "Need to restore first Point" << point;
    }
    else if(y < height){
        ff = 2;
        translateTo.setY((height - y) / _zoom);

        if(!isOkTranslate(translateTo, true)){
            translateTo.setY(0.);
        }
    }
}

/*
 * la funzione viene chiamata quando si zoom diminuendo lo zoom
 * per evitare problemi si controlla che il pixel più grosso all'interno della
 * pagina sia fuori dal pixmap, in larghezza, in caso sia dentro, si verifica se
 * è possibile spostare tutti i dati a destra, se si si sposta e si rifà il
 * controllo che siano fuori, in caso contrario si fa il return di false e
 * bisogna rifare il pixmap
*/
void datastruct::adjustWidth(cdouble width, QPointF& translatoTo)
{
    const QPointF point = this->getPointFirstPage();
    double biggerX = biggerx();
    bool not_used f;
    cdouble x = point.x();

    if(x < 0. && biggerX <= width){
        translatoTo.setX(width - biggerX);
        f = true;
        if(!isOkTranslate(translatoTo, true)){
            translatoTo.setX(0.);
        }

    }else{ //(x >= width)
        if(point.x() > 0.){
            f = false;
            translatoTo.setX(-point.x());
        }
    }

    translatoTo / _zoom;
}

/*
 * la funzione ritorna adjustWidth
 * the function consider the fact that the
 * height of one sheet is bigger than the width
*/
void datastruct::adjustAll(uint width,
                           uint height,
                           QPointF& res)
{
    res = {0., 0.};
    adjustWidth(width, res);
    adjustHeight(height, res);
}

void datastruct::adjustAll(const QSize &size, QPointF& res)
{
    this->adjustAll(size.width(), size.height(), res);
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
    if(un(point == QPointF(0, 0)))
        return;

    W_ASSERT(_pointFirstPage.x() + point.x() - prec <= 0.);
    W_ASSERT(_pointFirstPage.y() + point.y() - prec <= 0.);

    this->_pointFirstPage += point;
    this->_pageVisible = -1;

    if(likely(heightView > 0)){
        this->triggerVisibility(heightView);
    }else{
        cint range = this->get_range_visible();
        int first = this->getFirstPageVisible();

        if(un(first) < 0)
            first = 0;

        this->setVisible(first, first + range);
    }

    W_ASSERT(this->getPointFirstPageNoZoom().x() - prec <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() - prec <= 0.);
}

/* the list can be not order */
void datastruct::MovePoint(
        const QVector<int>  &pos,
        cint                pageIndex,
        const QPointF       &translation)
{
    Page &pageRef = at_mod(pageIndex);
    for(const auto &index : qAsConst(pos)){
        auto &stroke = pageRef.atStrokeMod(index);
        stroke.scale(translation);
    }
}

void datastruct::removePointIndex(
        QVector<int>    &pos,
        cint            __page,
        cbool           __isOrder)
{
    Page *page = &at_mod(__page);
    int i = pos.length() - 1;

    if(likely(__isOrder)){

#ifdef DEBUGINFO
        W_ASSERT(WCommonScript::is_order_vector(pos));
#else
        if(un(WCommonScript::is_order_vector(pos))){
            NAME_LOG_EXT->write("List not order", log_ui::critic_error);
            /* in this case we need to order */
            WCommonScript::order_vector(pos);
        }

#endif
    }else{
        WCommonScript::order_vector(pos);
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

void datastruct::removePage(const int pageIndex)
{
    int i, len;
    len = this->lengthPage();

    W_ASSERT(pageIndex >= 0 && pageIndex < len);

    this->_page.removeAt(pageIndex);

    len --;

    for(i = pageIndex; i < len; i++){
        auto &page = at_mod(i);
        page.setCount(i + 1);
    }

    this->_pageVisible = -1;
}

void datastruct::moveToPage(int newPage)
{
    const int range = this->get_range_visible();

    this->setPointFirstPage(QPointF(0., - at(newPage).minHeight()));

    W_ASSERT(this->getPointFirstPageNoZoom().x() <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() <= 0.);

    if(WCommonScript::debug_enable()){
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

    WCommonScript::abortIfDebug(__FILE__, __LINE__);
    return -1;
}

void datastruct::insertPage(const Page &Page, int index)
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
            const auto &Stroke1 = Page1.atStroke(k);
            const auto &Stroke2 = Page2.atStroke(k);

            if (Stroke1 != Stroke2) {
                return false;
            }
        }
    }

    return false;
}
