#pragma once

#include "core/WMutex.h"
#include "FileContainer/WZipWriterSingle.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "utils/common_def.h"
#include "utils/WCommonScript.h"
#include <pthread.h>
#include <memory>
#include "core/WImage.h"
#include "touch/dataTouch/stroke/StrokeForPage.h"
#include "core/WPixmap.h"
#include "core/WPen.h"
#include "core/RectF.h"
#include "core/Point.h"
#include "core/WSizeTemplate.h"
#include "core/WListFast.h"
#include "core/pointer/SharedPtr.h"
#include "core/WVector.h"

#define COLOR_NULL WColor::fromRgb(255, 255, 255, 255)
#define TEMP_COLOR WColor::fromRgb(105, 105, 105, 255)
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

#define Define_PAINTER_p(painter, ___img)               \
    WPainter painter;                                   \
    if(!painter.begin(&___img)) {                       \
        if constexpr(WCommonScript::debug_enable()){    \
            std::abort();                               \
        }                                               \
    };                                                  \
    SetRenderPainter(painter);

#define Define_PAINTER(painter) Define_PAINTER_p(painter, _imgDraw)

#define End_painter(painter) if(!painter.end()) { if(WCommonScript::debug_enable()){ std::abort(); }  };


enum n_style: int;

void adjustStrokePage(const WList<Stroke> &List, int count, Stroke *m_stroke);

constexpr bool debugPage = false;

class Page
{
private:
    static constexpr unsigned width = 1000; //1666;
    static constexpr double proportion = 1.4141;
    static constexpr unsigned height = width * proportion; // correct proportions for A4 paper size

    mutable WMutex                      _img;
    mutable WMutex                      _append_load;
    mutable bool                        _isVisible = true;
    int                                 _count;
    WListFast<SharedPtr<Stroke>>        _stroke;
    StrokeForPage                       _stroke_writernote;

    static constexpr auto pageDebug = true;

    void rep() const noexcept;

    /**
     * after adding data to the list, call triggernewimage,
     * and pass as all false, in this way what is
     * to be drawn will be drawn above the current image, and
     * then strokeTmp will be added to the stroke list
    */
    WListFast<SharedPtr<Stroke>>   _strokeTmp;
    mutable WPixmap                   _imgDraw;

    void drawNewPage(n_style style) noexcept;
    
    void drawEngine(WPainter &painter, WListFast<SharedPtr<Stroke>> &List, int m_pos_ris, bool use_multi_thread) noexcept;
    void draw(WPainter &painter, int m_pos_ris, bool all) noexcept;
    void drawStroke(WPainter &painter, const Stroke &stroke, WPen &pen, const WColor &color) const noexcept;

    void mergeList() noexcept;

    void AppendDirectly(const std::shared_ptr<Stroke>& stroke);
    bool initImg(bool flag);

    void decreaseAlfa(const WVector<int> &pos, WPainter *painter, int decrease);

    static PointF at_translation(const PointF &point, cint page);
    static RectF get_size_area(const WListFast<std::shared_ptr<Stroke>> & item, int from, int to);

public:
    const WPixmap &getImg() const;

    Page ();
    Page (Page &&other) noexcept;
    Page (const Page &from);
    Page (int count, n_style style);
    ~Page();

#define PAGE_SWAP_TRIGGER_VIEW BIT(1)
    void swap(WListFast<std::shared_ptr<Stroke>> &stroke, const WVector<int> & pos, int flag);
    void swap(WListFast<std::shared_ptr<Stroke>> &stroke, int from, int to);
    std::shared_ptr<Stroke> swap(int index, std::shared_ptr<Stroke> newData);

    bool updateFlag(const PointF &FirstPoint, double zoom, double heightView);
    void setVisible(bool vis) const;

    __fast int lengthStroke() const;

    bool isVisible() const;

    /**
     * \param pos to order positions of the stroke to be removed
     * */
    void removeAt(const WVector<int> & pos);
    void removeAt(int i);

    const Stroke & last() const;
    Stroke &lastMod();

    /**
     *  these 3 functions do not automatically launch
     *  the drawing of the whole sheet, they wait for
     *  the triggerRenderImage to be executed.
    */
    __fast void append(const std::shared_ptr<Stroke>& stroke);
    __fast void append(std::shared_ptr<Stroke> &&stroke);
    __fast void reserve(int stroke) { _strokeTmp.reserve(stroke); } ;
    // TODO --> make template
    __fast void append(const WList<std::shared_ptr<Stroke>> & stroke);
    __fast void append(const WListFast<std::shared_ptr<Stroke>> &stroke);

    [[deprecated]]
    __fast const Stroke             & atStroke(int i) const;
    [[deprecated]]
    __fast Stroke                   & atStrokeMod(int i);

    __fast const StrokeForPage &get_stroke_page() const; //return the point written by writernote
    __slow void at_draw_page(int IndexPoint, const PointF &translation, PointF &point, double zoom) const;

    auto minHeight() const -> double;
    auto currentHeight() const -> double;
    auto currentWidth() const -> double;

    auto userWrittenSomething() const -> bool;

    [[deprecated]]
    auto move(uint from, uint to) -> void;

    void triggerRenderImage(int m_pos_ris, bool all);

    auto getCount() const -> int;
    auto getIndex() const -> int;

    void reset();

    void drawStroke(const Stroke &stroke, int m_pos_ris);
    void drawForceColorStroke(const Stroke &stroke, cint m_pos_ris, const WColor &color, WPainter *painter);
    void drawForceColorStroke(const WVector<int> &pos, int m_pos_ris, const WColor &color);

    void removeAndDraw(int m_pos_ris, const WVector<int> &pos, const RectF &area);
    void drawIfInside(int m_pos_ris, const RectF &area);
    void decreaseAlfa(const WVector<int> &pos, int decrease);

    RectF get_size_area(const WVector<int> &pos) const;

    // block for appending
    void lock() const;
    void unlock() const;

    void setCount(int count);

    static auto write (WritableAbstract &writable, const Page &page, bool saveImage = true) -> int;
    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, Page>;

    //static void copy(const Page &src, Page &dest);
    constexpr static double getProportion();
    constexpr static double getHeight();
    constexpr static double getWidth();
    constexpr static WSize getResolutionSize();

    constexpr static PointF size();
    constexpr static PointF sizePoint();

    constexpr static double getResolutionWidth();
    constexpr static double getResolutionHeigth();

#define DR_IMG_INIT_IMG BIT(1) // init the image with a image transparent
    void drawToImage(const WVector<int> &index, WPixmap &img, cint flag) const;

    auto operator=(const Page &other) noexcept -> Page &;
    auto operator=(Page &&other) noexcept -> Page &;

    auto operator==(const Page &other) const noexcept -> bool;

    friend class StrokeNormal;
    friend class StrokeRect;
    friend class StrokeCircle;
    friend class StrokeLine;
    friend class Stroke;
    friend class PageFile;
    friend class stroke_drawer;
    friend class DataStruct;
    friend class xmlstruct;
    friend class RubberMethod;
    friend void * __page_load(void *);
    friend void adjustStrokePage(WList<Stroke> &List, int count, Stroke *m_stroke);
    friend class copy;
    friend void actionRubberSingleTotal(struct DataPrivateMuThread *_data);
};

force_inline void Page::rep() const noexcept {
#ifdef DEBUGINFO
    W_ASSERT(this->_count > 0);
#endif // DEBUGINFO
}

force_inline void Page::unlock() const
{
    this->_img.unlock();
}

force_inline void Page::lock() const
{
    this->_img.lock();
}

force_inline double Page::currentHeight() const
{
    return _count * Page::getHeight();
}

force_inline double Page::currentWidth() const
{
    return Page::getWidth();
}

force_inline void Page::move(const uint from, const uint to)
{
    this->_stroke.move(from, to);
}

force_inline int Page::getCount() const
{
    return this->_count;
}

force_inline void Page::reset()
{
    this->_stroke.clear();
    this->_stroke_writernote.reset();
    this->_isVisible = true;
    this->_count = -1;
    this->_strokeTmp.clear();
    this->_imgDraw = WPixmap(1, true);
}

inline PointF Page::at_translation(const PointF &point, cint page)
{
    PointF tmp;
    const double ytranslation = double(page) * Page::getHeight();

    if (un(!page)) {
        return point;
    }

    tmp = point;
    tmp.ry() -= ytranslation;
    return tmp;
}

force_inline int Page::getIndex() const
{
    return this->getCount() - 1;
}

force_inline void Page::AppendDirectly(const std::shared_ptr<Stroke>& stroke)
{
    this->_stroke.append(stroke);
}

force_inline const WPixmap &Page::getImg() const
{
    return this->_imgDraw;
}

constexpr force_inline double Page::getProportion()
{
    return proportion;
}

constexpr force_inline PointF Page::size()
{
    return {Page::getWidth(), Page::getHeight()};
}

constexpr force_inline double Page::getHeight()
{
    return height;
}

constexpr force_inline double Page::getWidth()
{
    return width;
}

constexpr force_inline auto Page::getResolutionSize() -> WSize
{
    return WSize(Page::getResolutionWidth(), Page::getResolutionHeigth());
}

constexpr force_inline double Page::getResolutionWidth()
{
    return getWidth() * PROP_RESOLUTION;
}

constexpr force_inline double Page::getResolutionHeigth()
{
    return getHeight() * PROP_RESOLUTION;
}

force_inline bool Page::updateFlag(
        const PointF    &FirstPoint,
        cdouble         zoom,
        cdouble         heightView)
{
    double heightSec;

    W_ASSERT(zoom >= 0);
    //W_ASSERT(FirstPoint.x() <= 0.0 and FirstPoint.y() <= 0.0);
    W_ASSERT(heightView >= 0);

    heightSec = Page::getHeight() * zoom;

    cdouble minH = heightSec * double(_count - 1) / zoom + FirstPoint.y();
    cdouble maxH = heightSec * double(_count)     / zoom + FirstPoint.y();

    if(heightView <= Page::getHeight() * zoom){
        // if the page is not fully visible in a window

        _isVisible = discordant(maxH, minH);

        if(_isVisible)
            goto ret;
    }

    _isVisible = WCommonScript::included(0.0, heightView, minH) or
                 WCommonScript::included(0.0, heightView, maxH);

ret:
    WDebug(debugPage, "count" << _count
                              << "minH" << minH
                              << "heightView" << heightView
                              << "maxH" << maxH
                              << "zoom" << zoom
                              << _isVisible);

    return _isVisible;
}

force_inline void Page::setVisible(bool vis) const
{
    this->_isVisible = vis;
}

force_inline const Stroke &Page::atStroke(int i) const
{
    rep();
    const auto res = this->_stroke.at(i);
    W_ASSERT(res);
    return *res;
}

force_inline Stroke &Page::atStrokeMod(int i)
{
    rep();
    return *this->_stroke.operator[](i);
}

force_inline const StrokeForPage &Page::get_stroke_page() const
{
    rep();
    return this->_stroke_writernote;
}

static force_inline void __at_draw_private(const PointF &from, PointF &to, const double zoom, const PointF &translation)
{
    to = from;

    to *= zoom;
    to += translation;
}

force_inline int Page::lengthStroke() const
{
    return _stroke.size();
}

force_inline bool Page::isVisible() const
{
    return this->_isVisible;
}

/*inline void Page::copy(
        const Page  &src,
        Page        &dest)
{
    dest.reset();

    dest._stroke.reserve(src._stroke.size());
    dest._strokeTmp.reserve(src._strokeTmp.size());

    for (const auto &s : src._stroke) {
        dest._stroke.append(s->clone());
    }

    for (const auto &s : src._strokeTmp) {
        dest._strokeTmp.append(s->clone());
    }

    dest._stroke                    = src._stroke;
    dest._stroke_writernote         = src._stroke_writernote;
    dest._strokeTmp                 = src._strokeTmp;
    dest._imgDraw                   = src._imgDraw;
    dest._IsVisible                 = src._IsVisible;
    dest._count                     = src._count;
}*/

force_inline void Page::removeAt(int i)
{
    W_ASSERT(!(i < 0 || i >= _stroke.size()));

    (void)this->_stroke.takeAt(i);
}

force_inline const Stroke &Page::last() const
{
    rep();
    return *this->_stroke.last();
}

inline Stroke &Page::lastMod()
{
    rep();
    return *this->_stroke.operator[](this->lengthStroke() - 1);
}

inline void Page::append(std::shared_ptr<Stroke> &&stroke)
{
    W_ASSERT(stroke and not stroke->isEmpty());

    rep();

    this->_strokeTmp.append(std::move(stroke));

    WDebug(pageDebug, this->_count);
}

force_inline void Page::append(const std::shared_ptr<Stroke>& strokeAppend)
{
    std::shared_ptr<Stroke> stroke = strokeAppend;
    return this->append(std::move(stroke));
}

force_inline double Page::minHeight() const
{
    return (this->_count - 1) * Page::height;
}

force_inline Page::Page(const Page &from)
    : _count(-1)
    , _imgDraw(1, true)
{
    *this = from;
}

inline Page::~Page() = default;

inline Page &Page::operator=(const Page &other) noexcept
{
    if(this == &other){
        return *this;
    }

    reset();

    _stroke.reserve(other._stroke.size());
    _strokeTmp.reserve(other._strokeTmp.size());

    for (const auto &s : other._stroke) {
        _stroke.append(s->clone());
    }

    for (const auto &s : other._strokeTmp) {
        _strokeTmp.append(std::move(s->clone()));
    }

    _stroke                    = other._stroke;
    _stroke_writernote         = other._stroke_writernote;
    _strokeTmp                 = other._strokeTmp;
    _imgDraw                   = other._imgDraw;
    _isVisible                 = other._isVisible;
    _count                     = other._count;
    return *this;
}

force_inline void Page::drawForceColorStroke(
        const Stroke    &stroke,
        cint            m_pos_ris,
        const WColor  &color,
        WPainter        *painter)
{
    WPen pen;
    const auto needDelete = (bool) (!painter);

    if (needDelete) {
        if(un(initImg(false)))
            return this->triggerRenderImage(m_pos_ris, true);

        WNew(painter, WPainter, ());

        if (!painter->begin(&this->_imgDraw))
            std::abort();
    }

    this->drawStroke(*painter, stroke, pen, color);
    W_ASSERT(painter->isActive());

    if (needDelete) {
        painter->end();
        WDelete(painter);
    }
}

force_inline constexpr PointF Page::sizePoint()
{
    return PointF {
            Page::size().x(),
            Page::size().y()
    };
}

inline void Page::append(const WListFast<std::shared_ptr<Stroke>> &stroke)
{
    for (auto &ref: stroke) {
        this->append(ref);
    }
}

inline auto Page::operator==(const Page &other) const noexcept -> bool
{
    return  this->_stroke == other._stroke &&
            this->_isVisible == other._isVisible &&
            this->_strokeTmp == other._strokeTmp &&
            this->_stroke_writernote == other._stroke_writernote &&
            this->_count == other._count;
}

inline auto Page::operator=(Page &&other) noexcept -> Page &
{
    this->_stroke = std::move(other._stroke);
    this->_count = other._count;
    this->_strokeTmp = std::move(other._strokeTmp);
    this->_stroke_writernote = std::move(other._stroke_writernote);
    this->_isVisible = other._isVisible;
    this->_imgDraw = std::move(other._imgDraw);
    return *this;
}

inline auto Page::load(const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, Page>
{
    Page result;

    if (versionController.getVersionPage() != 0)
        return {-1, result};

    static_assert_type(result._count, int);
    static_assert_type(result._isVisible, bool);

    if (readable.read(&result._count, sizeof (&result._count)) < 0)
        return {-1, result};

    if (readable.read(&result._isVisible, sizeof (result._isVisible)) < 0)
        return {-1, result};

    {
        auto [res, list] = WListFast<SharedPtr<Stroke>>::load (versionController, readable);
        if (res < 0)
            return {-1, result};

        result._stroke = std::move (list);
    }

    {
        auto [res, list] = WListFast<SharedPtr<Stroke>>::load (versionController, readable);
        if (res < 0)
            return {-1, result};
        result._strokeTmp = std::move (list);
    }

    {
        auto [res, str] = StrokeForPage::load (versionController, readable);
        if (res < 0)
            return {-1, result};
        result._stroke_writernote = std::move (str);
    }

    {
        auto [res, img] = WPixmap::load (versionController, readable);
        if (res < 0)
            return {-1, result};
        result._imgDraw = std::move (img);
    }

    return {0, result};
}

// TODO: move into cpp file
// TODO: add option to not write the image
inline auto Page::write(WritableAbstract &writable, const Page &page, bool saveImage) -> int
{
    if (writable.write(saveImage) < 0)
        return -1;

    if (WListFast<SharedPtr<Stroke>>::write(writable, page._stroke) < 0)
        return -1;

    if (writable.write(&page._count, sizeof(page._count)) < 0) return -1;
    if (writable.write(&page._isVisible, sizeof(page._isVisible)) < 0) return -1;

    if (WListFast<SharedPtr<Stroke>>::write(writable, page._stroke) < 0)
        return -1;

    if (WListFast<SharedPtr<Stroke>>::write(writable, page._strokeTmp) < 0)
        return -1;

    if (StrokeForPage::write(writable, page._stroke_writernote) < 0)
        return -1;

    if (saveImage && WPixmap::write(writable, page._imgDraw) < 0)
        return -1;

    return 0;
}
