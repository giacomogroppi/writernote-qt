﻿#include "Page.h"
#include "core/core.h"
#include "log/log_ui/log_ui.h"
#include "sheet/style_struct.h"
#include "utils/common_error_definition.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "PageFile.h"
#include "FileContainer/WZipWriterSingle.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"
#include "utils/time/current_time.h"
#include "Scheduler/Scheduler.h"
#include "utils/threadcount.h"

#define PAGE_THREAD_MAX 16

static force_inline double widthToPressure(double v) { return v/10.0; };

static force_inline void __initImg(WPixmap &img)
{
    img = WPixmap(1, true);
    W_ASSERT(!img.isNull());
    img.fill(color_transparent);
}

struct page_thread_data {
    WMutex                                  * append = nullptr;
    WPainter                                * painter = nullptr;
    const WListFast<SharedPtr<Stroke>>      * m_stroke = nullptr;
    AudioPosition                           m_pos_ris = AudioPosition::makeInvalid();
    const Page                              * parent = nullptr;
};

static bool initPage = false;

struct dPrivate {
    WVector<DataPrivateMuThread> threadData;
    WMutex drawMutex;
    WVector<Scheduler::Ptr<WTask>> tasks;

    ~dPrivate()
    {
        W_ASSERT(Scheduler::isStop());
        tasks.forAll(&Scheduler::Ptr<WTask>::release);
    }

} pageDraw;

class PageDrawTask: public WTask
{
    DataPrivateMuThread &_data;
public:
    explicit PageDrawTask (DataPrivateMuThread& data)
        : WTask(nullptr, WTask::NotDeleteLater)
        , _data(data)
    {

    }

    void run() final
    {
        auto *  extra = (struct page_thread_data *)_data.extra;
        WPixmap img;
        WPainterUnsafe painter;
        auto &mutex = *extra->append;
        auto positionAudio = extra->m_pos_ris;
        const Page *page = extra->parent;

        __initImg(img);

        painter.begin(&img);

        WDebug(false, "call");

        for(; _data.from < _data.to; _data.from ++){
            const Stroke &ref = *extra->m_stroke->at(_data.from);

            WDebug(false, "Page::__page_load pointer" << &ref);
            W_ASSERT(!ref.isEmpty());

            const auto &color = ref.getColor(
                    (positionAudio.isValid())
                    ? ((ref.getPositionAudio() > positionAudio) ? 4 : 1)
                    : 1
            );

            page->drawStroke(painter, ref, color);
        }

        End_painter(painter);

        // the source image has the same size as img
        {
            WMutexLocker guard(mutex);
            W_ASSERT(extra->painter->isActive());
            extra->painter->drawPixmap(
                    img.rect(),
                    img);
        }
    }
};

void Page::init()
{
    const auto numberOfThread = threadCount::count();
    pageDraw.tasks.reserve(numberOfThread);

    for (int i = 0; i < numberOfThread; i++) {
        pageDraw.threadData.append(DataPrivateMuThread());
        pageDraw.tasks.append(Scheduler::Ptr<PageDrawTask>::make(pageDraw.threadData[i]));
    }

    initPage = true;
}

Page::Page(Page &&other) noexcept
    : _isVisible(other._isVisible)
    , _count(other._count)
    , _stroke(std::move(other._stroke))
    , _stroke_writernote(std::move(other._stroke_writernote))
    , _strokeTmp(std::move(other._strokeTmp))
    , _imgDraw(std::move(other._imgDraw))
{
    W_ASSERT(initPage == true);
}

static void setStylePrivate(
    bool            &fast,
    n_style         res,
    style_struct_S  &style)
{
    if(res == n_style::empty){
        res = n_style::square;
    }

    if (res == n_style::line) {
        fast = true;

        style.nx = TEMP_N_X;
        style.ny = 0;
    } else if(res == n_style::white) {
        /* we set the color manually */
        style.colore = color_black;

        style.nx = 1;
        style.ny = 1;
        style.thickness = 1;
    } else {
        fast = true;

        style.nx = TEMP_SQUARE * (Page::getHeight() / Page::getWidth());
        style.ny = TEMP_SQUARE;
    }
}

Page::Page(const int count, const n_style style)
{
    this->_count = count;
    this->_isVisible = true;
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

void Page::drawNewPage(n_style __style) noexcept
{
    bool fast = false;
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

auto Page::swap(
        const WVector<int>      &pos,
        WFlags<SwapItemFlag::swapItemFlag>    flag
    ) -> WListFast<SharedPtr<Stroke>>
{
    WListFast<SharedPtr<Stroke>> result;
    RectF area;

    W_ASSERT_TEXT(WAbstractList::isSorted(pos), pos << "is not sorted");

    if (not (flag & SwapItemFlag::TriggerView)) {
        // TODO: implement
        for (const auto index : std::as_const(pos)) {
            result.append(this->swap(index, index + 1));
        }
        return {};
    }

    for (const auto ref : std::as_const(pos)) {
        auto &r = this->_stroke[ref];
        W_ASSERT(!r->isEmpty());
        result.append(r);
    }

    area = this->get_size_area(pos);

    this->removeAndDraw(AudioPosition::makeInvalid(), pos, area);

    return result;
}

/*
 * this function mantain the item already in list
*/
auto Page::swap(int from, int to) -> WListFast<SharedPtr<Stroke>>
{
    WListFast<SharedPtr<Stroke>> result;
    W_ASSERT(from >= to);

    DebugVariable<int> drop = 0;
    DebugVariable<WVector<int>> itemDrop;

    for(to --; from <= to; to --){
        result.append(_stroke.takeAt(to));

        if (debugPage) {
            drop ++;
            itemDrop.append(to);
        }
    }

    if (debugPage)
        WDebug(debugPage, "Page::swap" << _count - 1 << drop << "Item drop, list" << itemDrop);
}

auto Page::swap(int index, const SharedPtr<Stroke>& newData) -> SharedPtr<Stroke>
{
    SharedPtr<Stroke> res = this->_stroke.at(index);
    W_ASSERT(newData);

    _stroke[index] = newData;

    W_ASSERT(res.get());
    return res;
}

void Page::drawStroke(WPainter &painter, const Stroke& stroke) const noexcept
{
    stroke.draw(painter, false, _count - 1, PROP_RESOLUTION);
}

void Page::drawStroke(
        WPainter        &painter,
        const Stroke    &stroke,
        const WColor    &color) const noexcept
{
    const auto isRubber = (color == COLOR_NULL);
    const auto isHigh = stroke.getAlfa() < 255;
    const auto last_comp_mode = painter.compositionMode();
    constexpr not_used bool measureTime = false;
    constexpr not_used bool debColor = false;
    const int page = _count - 1;

    W_ASSERT(painter.isActive());

    if (isRubber) {
        painter.setCompositionMode(WPainter::CompositionMode_Clear);
    } else if(isHigh) {
        painter.setCompositionMode(WPainter::CompositionMode_SourceOver);
    }

    stroke.draw(painter, isRubber, page, PROP_RESOLUTION, color);

    painter.setCompositionMode(last_comp_mode);

    W_ASSERT(painter.isActive());
}

void Page::drawEngine(
        WPainter                        &painter,
        const WListFast<SharedPtr<Stroke>>    &strokes,
        AudioPosition                   m_pos_ris,
        bool                            use_multi_thread) noexcept
{
    int i;

    struct page_thread_data extraData;

    extraData.append        = &_append_load;
    extraData.painter       = &painter;
    extraData.m_stroke      = &strokes;
    extraData.m_pos_ris     = m_pos_ris;
    extraData.parent        = this;

    WMutexLocker guard(pageDraw.drawMutex);

    if (use_multi_thread) {
        auto threadCount = DataPrivateMuThreadInit(pageDraw.threadData, &extraData, PAGE_THREAD_MAX, strokes.size(), ~DATA_PRIVATE_FLAG_SEM);

        pageDraw.tasks.refMid(0, threadCount).forAll(&Scheduler::addTaskGeneric);
        pageDraw.tasks.refMid(0, threadCount).forAll(&WTask::join);
    } else {
        pageDraw.threadData[0].extra = &extraData;
        pageDraw.threadData[0].from = 0;
        pageDraw.threadData[0].to = strokes.size();

        pageDraw.tasks[0]->run();
    }
}

inline void Page::draw(
    WPainter        &painter,
    AudioPosition   m_pos_ris,
    bool            all) noexcept
{
    W_ASSERT(painter.isActive());

    if(all and _stroke.size()){
        this->drawEngine(painter, _stroke, m_pos_ris, true);
    }

    if (_strokeTmp.size()) {
        this->drawEngine(painter, _strokeTmp, m_pos_ris, false);
    }

    W_ASSERT(painter.isActive());

    this->mergeList();
}

void Page::mergeList() noexcept
{
    _stroke.append(std::move(_strokeTmp));
    W_ASSERT(_strokeTmp.isEmpty());
}

void Page::drawToImage(
    const WVector<int>      &indexes,
    WPixmap                 &img,
    WFlags<DrawToPageFlag::drawToPageFlag>  flag) const
{
    WPen pen;
    WPainterUnsafe painter;

    if (flag & DrawToPageFlag::initImage) {
        __initImg(img);
    }

    painter.begin(&img);

    for (const int index : std::as_const(indexes)) {
        const Stroke &stroke = atStroke(index);
        this->drawStroke(painter, stroke, stroke.getColor());
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

void Page::decreaseAlfa(const WVector<int> &pos, WPainter * painter, int decrease)
{
    for (const auto position: std::as_const(pos)) {
        Stroke &stroke = this->operator[](position);
        const auto color = stroke.getAlfa();

        stroke.setAlfaColor(color / decrease);

        if (painter) {
            this->drawStroke(*painter, stroke, COLOR_NULL);
            this->drawStroke(*painter, stroke, stroke.getColor());
        }
    }

    W_ASSERT(painter->isActive());
}

/**
 * all --> indicates if all the points must be drawn from scratch,
 * if false it is drawn over the old image
*/
void Page::triggerRenderImage(AudioPosition m_pos_ris, bool all)
{
    rep();

    all = initImg(all);

    WPainterUnsafe painter;
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
        AudioPosition       m_pos_ris,
        const WVector<int>  &pos,
        const RectF         &area)
{
    this->drawForceColorStroke(pos, m_pos_ris, COLOR_NULL);

    removeAt(pos.begin(), pos.end());

    drawIfInside(m_pos_ris, area);
}

void Page::drawIfInside(AudioPosition m_pos_ris, const RectF &area)
{
    if (not area.intersects(RectF {
        0, 0, getWidth(), getHeight()
    }))
        return;

    WPainterUnsafe painter;
    painter.begin(&this->_imgDraw);

    for (auto &stroke: *this) {
        if (is_inside_squade(stroke->getBiggerPointInStroke(), area))
            stroke->draw(painter, false, getIndex(), PROP_RESOLUTION);
    }

    End_painter(painter);
}

// let this method accept AudioPosition
void Page::decreaseAlfa(const WVector<int> &pos, int decrease)
{
    bool needInit = initImg(false);

    if (needInit) {
        WDebug(debugPage, "Warning: page not draw");
        W_ASSERT(0);
        return this->triggerRenderImage(AudioPosition::makeInvalid(), true);
    }

    WPainterUnsafe painter;
    painter.begin(&_imgDraw);

    this->decreaseAlfa(pos, &painter, decrease);

    End_painter(painter);
}

auto Page::get_size_area(const WListFast<SharedPtr<Stroke>> &strokes, int from, int to) -> RectF
{
    RectF result;

    if(from >= to){
        return {};
    }

    result = strokes.at(from)->getBiggerPointInStroke();

    for(; from < to; from ++){
        const RectF tmp = strokes.at(from)->getBiggerPointInStroke();
        result = DataStruct::joinRect(result, tmp);
    }

    return result;
}

Page::Page()
    : _img()
    , _isVisible(true)
    , _count(-1)
    , _stroke()
    , _strokeTmp()
{

}

auto Page::get_size_area(const WVector<int> &pos) const -> RectF
{
    if (pos.isEmpty())
        return {};

    RectF result(this->atStroke(pos.first()).getBiggerPointInStroke());

    for (const auto& index: std::as_const(pos)) {
        const auto &tmp = atStroke(index).getBiggerPointInStroke();
        result = DataStruct::joinRect(result, tmp);
    }

    return result;
}

void Page::setCount(int newCount)
{
    int delta = newCount - this->_count;
    cdouble deltaY = Page::getHeight() * delta;

    if (this->_count < 0) {
        _count = newCount;
        return;
    }

    this->_stroke.forAll([deltaY](SharedPtr<Stroke> &stroke) {
        stroke->scale({0., deltaY});
    });

    this->_stroke_writernote.scale(PointF(0., deltaY));

    this->_count = newCount;

    rep();
}

void Page::drawForceColorStroke(const WVector<int> &pos, AudioPosition m_pos_ris, const WColor &color)
{
    auto [error, painter] = initPainter(m_pos_ris);

    if (error)
        return;

    for (const auto &index: std::as_const(pos)) {
        const Stroke &stroke = atStroke(index);
        this->drawForceColorStroke(stroke, m_pos_ris, color, &painter);
    }

    W_ASSERT(painter.isActive());
    End_painter(painter);
}

void Page::drawStroke(const Stroke &stroke, AudioPosition m_pos_ris)
{
    auto [status, painter] = initPainter(m_pos_ris);

    if (status)
        return;

    drawStroke(painter, stroke);

    painter.end();
}

void Page::drawStroke(const WVector<int> &positions, AudioPosition positionAudio)
{
    auto [status, painter] = initPainter(positionAudio);

    if (status)
        return;

    for (const auto& index: std::as_const(positions)) {
        drawForceColorStroke(atStroke(index), positionAudio, atStroke(index).getColor(), &painter);
    }
}

void Page::at_draw_page(
        int             IndexPoint,
        const PointF    &translation,
        PointF          &point,
        const double    zoom) const
{
    const auto &stroke = get_stroke_page();
    const auto &p = stroke._data->_point.at(IndexPoint);

    __at_draw_private(p, point, zoom, translation);
}

auto Page::clearAudio() -> void
{
    for (auto& stroke: *this) {
        stroke->clearAudio();
    }
}
