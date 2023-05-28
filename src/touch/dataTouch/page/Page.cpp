#include "Page.h"
#include "core/core.h"
#include "log/log_ui/log_ui.h"
#include "sheet/style_struct.h"
#include "utils/common_error_definition.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "PageFile.h"
#include "core/WZipWriterSingle.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"
#include "utils/time/current_time.h"

#define PAGE_THREAD_MAX 16

static force_inline double widthToPressure(double v) { return v/10.0; };

static void setStylePrivate(
    bool            &fast,
    n_style         res,
    style_struct_S  &style)
{
    if(res == n_style::empty){
        res = n_style::square;
    }

    if(res == n_style::line){
        fast = true;

        style.nx = TEMP_N_X;
        style.ny = 0;
    }
    else if(res == n_style::white){
        /* we set the color manually */
        style.colore = color_black;

        style.nx = 1;
        style.ny = 1;
        style.thickness = 1;
    }else{
        fast = true;

        style.nx = TEMP_SQUARE * (Page::getHeight() / Page::getWidth());
        style.ny = TEMP_SQUARE;
    }
}

static force_inline void __initImg(WPixmap &img)
{
    img = WPixmap(1, true);
    W_ASSERT(!img.isNull());
    img.fill({color_transparent});
}

Page::Page(const int count, const n_style style)
{
    this->_count = count;
    this->_IsVisible = true;
    drawNewPage(style);
    this->mergeList();
}

static inline void drawLineOrizzontal(
        StrokeForPage   &stroke,
        style_struct_S  &style)
{
    constexpr auto initial = 20.;
    constexpr auto lastLine = Page::getHeight() - 20.;
    const auto delta = (lastLine - initial) / (double)style.nx;

    constexpr auto xLeft = 20.;
    constexpr auto xRight = Page::getWidth() - 20.;

    for (int i = 0; i < style.nx; i++) {
        const auto y = delta * double(i) + initial;

        const auto pointBegin = PointF {xLeft, y};
        const auto pointEnd =   PointF {xRight, y};

        stroke.append(pointBegin, stroke_append_default );
        stroke.append(pointEnd, stroke_append_default);
    }

    stroke.append({xLeft, Page::getHeight() - 20.}, stroke_append_default);
    stroke.append({xRight, Page::getHeight() - 20.}, stroke_append_default);
}

static inline void drawLineVertical(
        StrokeForPage   &stroke,
        style_struct_S  &style)
{
    constexpr auto initial = 20.;
    constexpr auto lastLine = Page::getWidth() - 20.;
    const auto delta = (lastLine - initial) / (double)style.ny;

    const auto yTop = 20.;
    const auto yButtom = Page::getHeight() - 20.;

    for (int i = 0; i < style.ny; i++) {
        const auto x = delta * double(i) + initial;

        const auto pointBegin = PointF {x, yTop};
        const auto pointEnd =   PointF {x, yButtom };

        stroke.append(pointBegin, stroke_append_default);
        stroke.append(pointEnd, stroke_append_default);
    }

    stroke.append({Page::getWidth() - 20., yTop}, stroke_append_default);
    stroke.append({Page::getWidth() - 20., yButtom}, stroke_append_default);
}

void Page::drawNewPage(n_style __style)
{
    bool fast = false;
    double deltax, deltay, ct_del;
    struct style_struct_S style;

    auto &stroke = this->_stroke_writernote;

    setStylePrivate(fast, __style, style);

    if (fast) {
        style.colore = TEMP_COLOR;
        style.thickness =  TEMP_TICK;
    }

    style.thickness = style.thickness >= 0. ? style.thickness : 1.;
    style.nx = style.nx >= 0 ? style.nx : 1;
    style.ny = style.ny >= 0 ? style.ny : 1;

    stroke.setMetadata(style.colore);

    drawLineOrizzontal( stroke, style);
    drawLineVertical(   stroke, style);

    stroke.setPressure(widthToPressure(style.thickness));
}

void Page::swap(WListFast<std::shared_ptr<Stroke>> &list,
        const WVector<int>  &pos,
        int                 flag)
{
    RectF area;

#ifdef DEBUGINFO
    if(un(!pos.isOrder())){
        qDebug() << "List not order" << __FILE__ << __FUNCTION__;
    }
#endif

    if(!(flag & PAGE_SWAP_TRIGGER_VIEW)){
        for (const auto index : std::as_const(pos)) {
            this->swap(list, index, index + 1);
        }
        return;
    }

    for(cint ref : pos){
        auto &r = this->_stroke[ref];
        W_ASSERT(!r->isEmpty());
        list.append(r);
    }

    area = this->get_size_area(pos);

    this->removeAndDraw(-1, pos, area);
}

/*
 * this function mantain the item already in list
*/
void Page::swap(WListFast<std::shared_ptr<Stroke> > &list,
                int             from,
                int             to)
{
    W_ASSERT(from >= to);
    DO_IF_DEBUG(
    int drop = 0;
    WListFast<int> itemDrop;
    );

    for(to --; from <= to; to --){
        list.append(_stroke.takeAt(to));

        DO_IF_DEBUG_ENABLE(debugPage, drop ++);
        DO_IF_DEBUG_ENABLE(debugPage, itemDrop.append(to));
    }

    DO_IF_DEBUG_ENABLE(debugPage,
        qDebug() << "Page::swap" << _count - 1 << drop << "Item drop, list" << itemDrop;
            );
}

std::shared_ptr<Stroke> Page::swap(int index, std::shared_ptr<Stroke> newData)
{
    std::shared_ptr<Stroke> res = this->_stroke.at(index);
    W_ASSERT(newData);

    _stroke[index] = newData;

    W_ASSERT(res.get());
    return res;
}

size_t Page::get_size_in_file(cbool saveImg) const
{
    return PageFile::size_in_file(*this, saveImg);
}

/* the list should be order */
void Page::removeAt(const WVector<int> &pos)
{
    int i;
    if(!pos.isOrder()){
        DO_IF_DEBUG(std::abort());
        auto &vec = (WVector<int> &)pos;
        vec.order();
    }

    i = pos.size();
    for(i--; i >= 0; i--){
        this->removeAt(i);
    }
}

void Page::append(const WList<std::shared_ptr<Stroke>> &stroke)
{
    for (const auto & tmp : std::as_const(stroke)) {
        this->append(tmp);
    }
}

void Page::drawStroke(
        WPainter        &painter,
        const Stroke    &stroke,
        WPen            &m_pen,
        const WColor    &color) const
{
    cbool isRubber = (color == COLOR_NULL);
    cbool isHigh = stroke.get_alfa() < 255;
    const auto last_comp_mode = painter.compositionMode();
    constexpr not_used bool measureTime = false;
    constexpr not_used bool debColor = false;
    cint page = _count - 1;

    m_pen.setColor(color);

    W_ASSERT(painter.isActive());

    if (un(isRubber)) {
        painter.setCompositionMode(WPainter::CompositionMode_Clear);
    } else if(isHigh) {
        painter.setCompositionMode(WPainter::CompositionMode_SourceOver);
    }

    stroke.draw(painter, isRubber, page, m_pen, PROP_RESOLUTION);

    if (un(isRubber)) {
        painter.setCompositionMode(WPainter::CompositionMode_SourceOver);
    }

    painter.setCompositionMode(last_comp_mode);

    W_ASSERT(painter.isActive());
}

struct page_thread_data{
    WMutex                          * append;
    WPainter                        * painter;
    WListFast<std::shared_ptr<Stroke>>  * m_stroke;
    int                             m_pos_ris;
    const Page                      * parent;
};

/*The only way to draw in thread safe on a WPainter is to draw an image,
 *  as it is very fast compared to drawing the stroke, first we draw
 *  all the strokes on the images, residing in the thread stack stacks,
 *  and then we draw them on top the original painter
*/
void * __page_load(void *__data)
{
    auto *  _data = (struct DataPrivateMuThread *)__data;
    auto *  extra = (struct page_thread_data *)_data->extra;
    WPixmap img;
    WPen m_pen;
    WPainter painter;
    auto &mutex = *extra->append;
    int m_pos_ris = extra->m_pos_ris;
    const Page *page = extra->parent;

    __initImg(img);

    painter.begin(&img);

    WDebug(false, "call");

    for(; _data->from < _data->to; _data->from ++){
        const Stroke &ref = *extra->m_stroke->at(_data->from);

        WDebug(false, "Page::__page_load pointer" << &ref);
        W_ASSERT(!ref.isEmpty());

        const WColor &color = ref.getColor(
            (un(m_pos_ris != -1))
                    ?
                        (
                          (ref.getPosizioneAudio() > m_pos_ris)
                            ? 4
                            : 1
                        )
                    :
                        1
        );

        page->drawStroke(painter, ref, m_pen, color);
    }

    End_painter(painter);

    // the source image has the same size as img
    mutex.lock();

    W_ASSERT(extra->painter->isActive());
    extra->painter->drawPixmap(
            img.rect(),
            img);
    mutex.unlock();

    return nullptr;
}

void Page::drawEngine(
        WPainter        &painter,
        WListFast<std::shared_ptr<Stroke>> &List,
        int             m_pos_ris,
        bool            use_multi_thread)
{
    int i, threadCount;

    pthread_t thread[PAGE_THREAD_MAX];
    struct DataPrivateMuThread threadData[PAGE_THREAD_MAX];
    struct page_thread_data extraData;

    extraData.append        = &_append_load;
    extraData.painter       = &painter;
    extraData.m_stroke      = &List;
    extraData.m_pos_ris     = m_pos_ris;
    extraData.parent        = this;

    if (use_multi_thread) {
        threadCount = DataPrivateMuThreadInit(threadData, &extraData, PAGE_THREAD_MAX, List.size(), ~DATA_PRIVATE_FLAG_SEM);

        for (i = 0; i < threadCount; i++) {
            pthread_create(&thread[i], nullptr, __page_load, &threadData[i]);
        }
        for (i = 0; i < threadCount; i++) {
            pthread_join(thread[i], nullptr);
        }
    } else {
        threadData->extra   = &extraData;
        threadData->from    = 0;
        threadData->to      = List.size();

        __page_load(&threadData[0]);
    }
}

inline void Page::draw(
    WPainter    &painter,
    int         m_pos_ris,
    bool        all)
{
    W_ASSERT(painter.isActive());

    if(un(all) and _stroke.size()){
        this->drawEngine(painter, _stroke, m_pos_ris, true);
    }

    if(_strokeTmp.size()){
        this->drawEngine(painter, _strokeTmp, m_pos_ris, false);
    }

    W_ASSERT(painter.isActive());

    this->mergeList();
}

void Page::mergeList()
{
    for(auto &s : _strokeTmp) {
        W_ASSERT(!s->isEmpty());
        _stroke.append(s);
        W_ASSERT(*_stroke.last() == *s);
    }

    _strokeTmp.clear();
}

void Page::drawToImage(
    const WVector<int>  &index,
    WPixmap              &img,
    cint                flag) const
{
    WPen pen;
    WPainter painter;

    if(un(flag & DR_IMG_INIT_IMG)){
        __initImg(img);
    }else{
        W_ASSERT(flag == ~DR_IMG_INIT_IMG);
    }

    painter.begin(&img);

    for (const int __index : std::as_const(index)){
        const Stroke &stroke = atStroke(__index);
        this->drawStroke(painter, stroke, pen, stroke.getColor());
    }

    W_ASSERT(painter.isActive());
    End_painter(painter);
}

bool Page::userWrittenSomething() const
{
    return this->_stroke.size();
}

bool Page::initImg(bool flag)
{
    flag = _imgDraw.isNull() || flag;
    if(flag)
        __initImg(_imgDraw);

    return flag;
}

void Page::decreseAlfa(const WVector<int> &pos, WPainter * painter, int decrese)
{
    int i = pos.size();
    uint color;
    WPen pen;

    for (i --; i >= 0; i--) {
        Stroke &stroke = atStrokeMod(pos.at(i));
        color = stroke.get_alfa();

        stroke.setAlfaColor(color / decrese);

        if (painter) {
            this->drawStroke(*painter, stroke, pen, COLOR_NULL);
            this->drawStroke(*painter, stroke, pen, stroke.getColor());
        }
    }

    W_ASSERT(painter->isActive());
}

/**
 * all --> indicates if all the points must be drawn from scratch,
 * if false it is drawn over the old image
*/
void Page::triggerRenderImage(int m_pos_ris, bool all)
{
    rep();

    all = initImg(all);

    WPainter painter;
    painter.begin(&_imgDraw);

    this->draw(painter, m_pos_ris, all);

    W_ASSERT(painter.isActive());
    End_painter(painter);

//#define PAGE_DEBUG_IMG
#ifdef PAGE_DEBUG_IMG
#ifdef Q_OS_LINUX
    const WString path = "~/Scrivania/tmp_foto";
#else
    const WString path = "/Users/giacomo/Desktop/tmp_foto";
#endif
    if(!this->_imgDraw.save(path + current_time_string() + ".png", "PNG", -1))
        std::abort();
#endif
}

#define MakeCTRL(point) \
    if(datastruct_isinside(topLeft1, bottomRight1, point)) \
        return true;

static force_inline int __is_inside_square(
        const RectF    &rect1,
        const RectF    &rect2)
{
    const PointF &topLeft1     = rect1.topLeft();
    const PointF &bottomRight1 = rect1.bottomRight();

    MakeCTRL(rect2.bottomRight());
    MakeCTRL(rect2.bottomLeft());

    MakeCTRL(rect2.topRight());
    MakeCTRL(rect2.topLeft());

    return false;
}

static force_inline int is_inside_squade(
        const RectF    &rect1,
        const RectF    &rect2)
{
    // testing
    return rect1.intersects(rect2);
    return __is_inside_square(rect1, rect2) || __is_inside_square(rect2, rect1);
}

void Page::removeAndDraw(
        int                 m_pos_ris,
        const WVector<int>  &pos,
        const RectF        &area)
{
    int i = pos.size();
    this->drawForceColorStroke(pos, m_pos_ris, COLOR_NULL);

    for(i --; i >= 0; i --){
        removeAt(pos.at(i));
    }

    drawIfInside(m_pos_ris, area);
}

void Page::drawIfInside(int m_pos_ris, const RectF &area)
{
    int index = lengthStroke() - 1;
    WPainter painter;
    painter.begin(&this->_imgDraw);

    for(; index >= 0; index --){
        const Stroke &stroke = this->atStroke(index);

        if(un(is_inside_squade(stroke.getBiggerPointInStroke(), area))){
            drawForceColorStroke(stroke, m_pos_ris, stroke.getColor(1.0), &painter);
        }
    }

    End_painter(painter);
}

#define PAGE_DRAW_SQUARE_ADJUST(point, function) \
    point.rx() = rect.function().x(); \
    point.ry() = rect.function().y(); \
    at_translation(point, this->_count - 1);

void Page::drawSquare(const RectF &rect)
{
    RectF tmp;
    //QBrush brush(COLOR_NULL, Qt::SolidPattern);
    WPen pen;
    WPainter painter;

    painter.begin(&_imgDraw);

    // we need to adjust the rect to our img
    {
        PointF point1, point2;
        PAGE_DRAW_SQUARE_ADJUST(point1, topLeft);
        PAGE_DRAW_SQUARE_ADJUST(point2, bottomRight);

        tmp = RectF(point1, point2);
    }

    pen.setColorNull();
    pen.setSolidPattern();

    painter.setPen(pen);
    painter.setCompositionClear();
    painter.fillRect(tmp.castTo<double>());
    End_painter(painter);
}

void Page::decreseAlfa(const WVector<int> &pos, int decrese)
{
    bool needInit = initImg(false);

    if (un(needInit)) {
        WDebug(debugPage, "Warning: page not draw");
        W_ASSERT(0);
        return this->triggerRenderImage(-1, true);
    }

    WPainter painter;
    painter.begin(&_imgDraw);

    this->decreseAlfa(pos, &painter, decrese);

    End_painter(painter);
}

RectF Page::get_size_area(const WListFast<std::shared_ptr<Stroke> > &item, int from, int to)
{
    RectF result;

    if(un(from >= to)){
        return {};
    }

    result = item.at(from)->getBiggerPointInStroke();

    for(; from < to; from ++){
        const RectF tmp = item.at(from)->getBiggerPointInStroke();
        result = DataStruct::get_bigger_rect(result, tmp);
    }

    return result;
}

Page::Page()
{
    this->_count = -1;
}

RectF Page::get_size_area(const WVector<int> &pos) const
{
    RectF result, tmp;
    int len = pos.size();

    if(un(!len)){
        return {};
    }

    len --;
    result = atStroke(pos.first()).getBiggerPointInStroke();

    for(; len >= 0; len --){
        tmp = atStroke(pos.at(len)).getBiggerPointInStroke();
        result = DataStruct::get_bigger_rect(result, tmp);
    }

    return result;
}

void Page::setCount(int newCount)
{
    int i = this->lengthStroke();
    int delta = newCount - this->_count;
    cdouble deltaY = Page::getHeight() * delta;

    if(un(this->_count < 0)){
        _count = newCount;
        return;
    }

    for(i --; i >= 0; i--){
        this->atStrokeMod(i).scale(PointF(0., deltaY));
    }

    this->_stroke_writernote.scale(PointF(0., deltaY));

    this->_count = newCount;

    rep();
}

void Page::drawForceColorStroke(const WVector<int> &pos, int m_pos_ris, const WColor &color)
{
    if(initImg(false))
        return this->triggerRenderImage(m_pos_ris, true);

    WPainter painter;
    painter.begin(&_imgDraw);

    for(const auto &index : std::as_const(pos)){
        const Stroke &stroke = atStroke(index);
        this->drawForceColorStroke(stroke, m_pos_ris, color, &painter);
    }

    W_ASSERT(painter.isActive());
    End_painter(painter);
}

#define DO_CTRL(function) \
    err = function; \
    if(un(err != OK)) \
        return err;

int Page::save(WZipWriterSingle &writer, cbool saveImg) const
{
    return PageFile::save(this, writer, saveImg);
}

int Page::load(WZipReaderSingle &reader, int ver_stroke)
{
    return PageFile::load(*this, ver_stroke, reader);
}

void Page::drawStroke(const Stroke &stroke, int m_pos_ris)
{
    drawForceColorStroke(stroke, m_pos_ris, stroke.getColor(1.0), nullptr);
}

void Page::at_draw_page(
        int            IndexPoint,
        const PointF   &translation,
        PointF         &point,
        const double    zoom) const
{
    const auto &stroke = get_stroke_page();
    const auto &p = stroke._data->_point.at(IndexPoint);

    __at_draw_private(p, point, zoom, translation);
}
