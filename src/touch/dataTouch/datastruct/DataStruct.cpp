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
    if (!DataStruct::isOkZoom(zoom)) {
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
    const auto startPage = whichPage(area.top());
    const auto bottomPage = whichPage(area.bottom());

    for (auto b = _page.begin() + startPage; b != _page.end() + bottomPage; b++)
        b->drawIfInside(AudioPosition::makeInvalid(), area);
}

DataStruct::DataStruct()
    : _last_translation(0., 0.)
    , _zoom(1.)
    , _pageVisible(-1)
{
}

void DataStruct::triggerIfNone(AudioPosition audioPosition)
{
    for (auto &page : *this) {
        if(page._imgDraw.isNull())
            page.triggerRenderImage(audioPosition, true);
    }
}

void DataStruct::reset_touch()
{
    _page.clear();
    _pageVisible = -1;
    _pointFirstPage = PointF(0, 0);
    _zoom = 1.;
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
    double y = this->_page.size() * Page::height;

    if (point.y() > 0.0) {
        ff = 1;
        translateTo.setY( - point.y());
        WDebug(debug, "Need to restore first point" << point);
    } else if(y < height) {
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

    _last_translation *= -1;

    scala_all(_last_translation, heightView);

    _last_translation = PointF(0, 0);
}

void DataStruct::scala_all(const PointF &point, double heightView)
{
    constexpr double prec = .00005;
    if (point == PointF(0, 0))
        return;

    W_ASSERT(_pointFirstPage.x() + point.x() - prec <= 0.);
    W_ASSERT(_pointFirstPage.y() + point.y() - prec <= 0.);

    this->_pointFirstPage += point;
    this->_pageVisible = -1;

    if (heightView > 0) {
        this->triggerVisibility(heightView);
    } else {
        cint range = this->get_range_visible();
        int first = this->getFirstPageVisible();

        if(first < 0)
            first = 0;

        this->setVisible(first, first + range);
    }

    W_ASSERT(this->getPointFirstPageNoZoom().x() - prec <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() - prec <= 0.);
}

/* the list can be not order */
void DataStruct::movePoint(
        const WVector<int>  &pos,
        int pageIndex,
        const PointF       &translation)
{
    Page &pageRef = this->operator[](pageIndex);
    for(const auto &index : std::as_const(pos)){
        auto &stroke = pageRef[index];
        stroke.scale(translation);
    }
}

void DataStruct::removePointIndex(
        WVector<int>    &positions,
        cint            page,
        cbool           isOrder)
{
    if (not isOrder)
        WAbstractList::sort(positions.begin(), positions.end());

    this->operator[](page).removeAt(positions.begin(), positions.end());
}

void DataStruct::removePage(int pageIndex)
{
    W_ASSERT(pageIndex >= 0 and pageIndex < lengthPage());

    this->_page.removeAt(pageIndex);

    for(auto i = pageIndex; i < lengthPage(); i++)
        this->operator[](i).setCount(i + 1);

    this->_pageVisible = -1;
}

void DataStruct::moveToPage(int newPage)
{
    const int range = this->get_range_visible();

    this->setPointFirstPage(PointF(0., - at(newPage).minHeight()));

    W_ASSERT(this->getPointFirstPageNoZoom().x() <= 0.);
    W_ASSERT(this->getPointFirstPageNoZoom().y() <= 0.);

    if constexpr (WUtils::debug_enable()) {
        const auto not_used point = this->adjustPoint(PointF(0., 0.));
        const auto not_used index = this->whichPage(point);
        WDebug(debug, newPage << index << this->getPointFirstPage() << get_range_visible());
        W_ASSERT(index == newPage);
    }

    this->setVisible(newPage, newPage + range);
}

int DataStruct::getLastPageVisible() const
{
    auto i = lengthPage();

    for(i --; i >= 0; i--){
        if(at(i).isVisible()){
            W_ASSERT(this->getFirstPageVisible() <= i);
            return i;
        }
    }

    WUtils::abortIfDebug(__FILE__, __LINE__);
    return -1;
}

void DataStruct::insertPage(const Page &Page, int index)
{
    this->_page.insert(index, Page);

    for (index ++; index < lengthPage(); index ++) {
        this->operator[](index).setCount(index + 1);
    }
}

bool DataStruct::userWrittenSomething(int fromPage)
{
    const auto len = lengthPage();
    for(; fromPage < len; fromPage++){
        if(at(fromPage).userWrittenSomething())
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

DataStruct &DataStruct::operator=(const DataStruct &other) noexcept
{
    if (this == &other)
        return *this;

    this->_pageVisible = other._pageVisible;
    this->_zoom = other._zoom;
    this->_last_translation = other._last_translation;
    this->_page = other._page;
    this->_pointFirstPage = other._pointFirstPage;

    W_ASSERT(*this == other);

    return *this;
}

DataStruct &DataStruct::operator=(DataStruct &&other) noexcept
{
    if (this == &other)
        return *this;

    this->_pageVisible = other._pageVisible;
    this->_zoom = other._zoom;
    this->_last_translation = other._last_translation;
    this->_page = std::move(other._page);
    this->_pointFirstPage = other._pointFirstPage;

    other.reset_touch();

    W_ASSERT(other.isEmptyTouch());

    return *this;
}

DataStruct::DataStruct(DataStruct &&other) noexcept
    : _last_translation(std::move(other._last_translation))
    , _page(std::move (other._page))
    , _pointFirstPage(std::move(other._pointFirstPage))
    , _zoom(std::move(other._zoom))
    , _pageVisible(std::move(other._pageVisible))
{
    other.reset_touch();
}

DataStruct::DataStruct(const DataStruct &other) noexcept
    : _last_translation(other._last_translation)
    , _page(other._page)
    , _pointFirstPage(other._pointFirstPage)
    , _zoom(other._zoom)
    , _pageVisible(other._pageVisible)
{
}

auto DataStruct::clearAudio() -> void
{
    for (auto& page: *this) {
        page.clearAudio();
    }
}

bool DataStruct::operator==(const DataStruct &other) const
{
    if (this == &other)
        return true;
    return  this->_pageVisible == other._pageVisible &&
            this->_zoom == other._zoom &&
            this->_last_translation == other._last_translation &&
            this->_page == other._page &&
            this->_pointFirstPage == other._pointFirstPage;
}
