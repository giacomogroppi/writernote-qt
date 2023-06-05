#include "DataStruct.h"
#include "core/WListFast.h"
#include "core/core.h"
#include "sheet/style_struct.h"
#include "frompdf/frompdf.h"
#include "images/fromimage.h"
#include "log/log_ui/log_ui.h"

#define mov_if_neg(p, x) \
    p = x; \
    return;

void DataStruct::newPage(int num)
{
    while (num > 0){
        this->newPage(n_style::white);
        num --;
    }
}

void DataStruct::changeZoom(double zoom,
                            int heightScreen)
{
    if(un(!DataStruct::isOkZoom(zoom))){
        return;
    }

    this->_zoom = zoom;

    this->triggerVisibility(heightScreen);
    this->_pageVisible = -1;
    (void)this->getFirstPageVisible();
}

void DataStruct::increaseZoom(const double delta, const WSizeF &size, PointF &res)
{
    this->_zoom += delta;
    this->adjustAll(size, res);
    this->triggerVisibility(size.getHeight());
    this->_pageVisible = -1;
}

void DataStruct::drawIfInside(const RectF &area)
{
    int i = this->getFirstPageVisible();

    for(; i >= 0; i--){
        Page *page = &at_mod(i);

        if(!page->isVisible())
            break;

        page->drawIfInside(-1, area);
    }
}

DataStruct::DataStruct()
{
    _last_translation = PointF(0., 0.);
    _zoom = 1.;
}

void DataStruct::triggerIfNone(int m_pos_ris)
{
    for (auto &page : _page) {
        if(page._imgDraw.isNull())
            page.triggerRenderImage(m_pos_ris, true);
    }
}

void DataStruct::reset_touch()
{
    _page.clear();
    _pageVisible = -1;
    _pointFirstPage = PointF(0, 0);
    _zoom = 1.;
}

// TODO: remove this method
void DataStruct::copy(const DataStruct &src, DataStruct &dest)
{
    int i;
    const int len = src.lengthPage();
    int diff = len - dest.lengthPage();

    if(diff > 0){
        for(i = 0; i < diff; i++){
            dest.newPage(n_style::white);
        }
    } else if(diff != 0){
        i = - diff - 1;

        for(; i >= 0; i--){
            dest.removePage(i);
        }
    }

    for(i = 0; i < len; i++){
        dest._page.operator[](i) = src._page.at(i);
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
void DataStruct::adjustHeight(cdouble height, PointF& translateTo)
{
    const PointF point = this->getPointFirstPageNoZoom();
    unsigned char not_used ff = 0;
    double y = biggery();

    if(point.y() > 0.0){
        ff = 1;
        translateTo.setY( - point.y());
        WDebug(true, "Need to restore first point" << point);
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
void DataStruct::adjustWidth(cdouble width, PointF& translatoTo)
{
    const PointF point = this->getPointFirstPage();
    double biggerX = this->biggerX();
    bool not_used f;
    cdouble x = point.x();

    if(x < 0. and biggerX <= width){
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
void DataStruct::adjustAll(double width,
                           double height,
                           PointF& res)
{
    res = {0., 0.};
    adjustWidth(width, res);
    adjustHeight(height, res);
}

void DataStruct::adjustAll(const WSizeF &size, PointF& res)
{
    this->adjustAll(size.getWidth(), size.getHeight(), res);
}

void DataStruct::restoreLastTranslation(const int heightView)
{
    if(_last_translation == PointF(0, 0))
        return;

    DataStruct::inverso(_last_translation);

    scala_all(_last_translation, heightView);

    _last_translation = PointF(0, 0);
}

void DataStruct::scala_all(const PointF &point, double heightView)
{
    constexpr double prec = .00005;
    if(un(point == PointF(0, 0)))
        return;

    W_ASSERT(_pointFirstPage.x() + point.x() - prec <= 0.);
    W_ASSERT(_pointFirstPage.y() + point.y() - prec <= 0.);

    this->_pointFirstPage += point;
    this->_pageVisible = -1;

    if(heightView > 0){
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
void DataStruct::MovePoint(
        const WVector<int>  &pos,
        cint                pageIndex,
        const PointF       &translation)
{
    Page &pageRef = at_mod(pageIndex);
    for(const auto &index : std::as_const(pos)){
        auto &stroke = pageRef.atStrokeMod(index);
        stroke.scale(translation);
    }
}

void DataStruct::removePointIndex(
        WVector<int>    &pos,
        cint            __page,
        cbool           __isOrder)
{
    Page *page = &at_mod(__page);
    int i = pos.size() - 1;

    if(__isOrder){

#ifdef DEBUGINFO
        W_ASSERT(pos.isOrder());
#else
        if(!pos.isOrder()){
            NAME_LOG_EXT->write("List not order", log_ui::critic_error);
            /* in this case we need to order */
            pos.order();
        }

#endif
    }else{
        pos.order();
    }

    for(; i >= 0; i --){
        page->removeAt(pos.at(i));
    }
}

void DataStruct::removePointIndex(
        WListFast<WVector<int> >    &pos,
        cint                    base,
        cbool                   __isOrder)
{
    int i, lenList;
    lenList = pos.size();

    for(i = 0; i < lenList; i++){
        removePointIndex(pos.operator[](i), base + i, __isOrder);
    }

}

void DataStruct::removePage(const int pageIndex)
{
    int i, len;
    len = this->lengthPage();

    W_ASSERT(pageIndex >= 0 and pageIndex < len);

    this->_page.removeAt(pageIndex);

    len --;

    for(i = pageIndex; i < len; i++){
        auto &page = at_mod(i);
        page.setCount(i + 1);
    }

    this->_pageVisible = -1;
}

void DataStruct::moveToPage(int newPage)
{
    const int range = this->get_range_visible();

    this->setPointFirstPage(PointF(0., - at(newPage).minHeight()));

    W_ASSERT(this->getPointFirstPageNoZoom().x() <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() <= 0.);

    if(WCommonScript::debug_enable()){
        const auto not_used point = this->adjustPoint(PointF(0., 0.));
        const auto not_used index = this->whichPage(point);
        WDebug(true, newPage << index << this->getPointFirstPage() << get_range_visible());
        W_ASSERT(index == newPage);
    }

    this->setVisible(newPage, newPage + range);
}

int DataStruct::getLastPageVisible() const
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

void DataStruct::insertPage(const Page &Page, int index)
{
    int len;

    this->_page.insert(index, Page);

    len = this->lengthPage();

    for(index ++; index < len; index ++){
        this->at_mod(index).setCount(index + 1);
    }
}

bool DataStruct::userWrittenSomething(uint frompage)
{
    const uint len = lengthPage();
    for(; frompage < len; frompage++){
        if(at(frompage).userWrittenSomething())
            return true;
    }

    return false;
}

bool DataStruct::userWrittenSomething() const
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
bool DataStruct::userWrittenSomething(const DataStruct &data1, const DataStruct &data2)
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
