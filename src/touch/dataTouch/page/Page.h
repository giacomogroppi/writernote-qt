#pragma once

#include "core/WMutex.h"
#include "FileContainer/WZipWriterSingle.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "utils/common_def.h"
#include "utils/WCommonScript.h"
#include <pthread.h>
#include <memory>
#include "core/Image/WImage.h"
#include "touch/dataTouch/stroke/StrokeForPage.h"
#include "core/Pixmap/WPixmap.h"
#include "core/Pen/WPen.h"
#include "core/RectF.h"
#include "core/Point.h"
#include "core/WSizeTemplate.h"
#include "core/WListFast.h"
#include "core/pointer/SharedPtr.h"
#include "core/WVector.h"
#include "core/WPainter/WPainterUnsafe.h"
#include "utils/WCommonScript.h"

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

#define End_painter(painter) if(!painter.end()) { if(WUtils::debug_enable()){ std::abort(); }  };


enum n_style: int;

void adjustStrokePage(const WList<Stroke> &List, int count, Stroke *m_stroke);

// TODO: move this variable into Page
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

    static constexpr auto pageDebug = false;

    void rep() const noexcept;

    /**
     * after adding data to the list, call triggernewimage,
     * and pass as all false, in this way what is
     * to be drawn will be drawn above the current image, and
     * then strokeTmp will be added to the stroke list
    */
    WListFast<SharedPtr<Stroke>> _strokeTmp;
    mutable WPixmap _imgDraw;

    void drawNewPage(n_style style) noexcept;
    
    void drawEngine(WPainter &painter, const WListFast<SharedPtr<Stroke>> &strokes, AudioPosition m_pos_ris, bool use_multi_thread) noexcept;
    void draw(WPainter &painter, AudioPosition m_pos_ris, bool all) noexcept;
    void drawStroke(WPainter &painter, const Stroke& stroke, const WColor &color) const noexcept;

    /**
     * \brief This method draw the stroke pass as parameter with this color.
     * */
    void drawStroke(WPainter &painter, const Stroke& stroke) const noexcept;

    void mergeList() noexcept;

    void appendDirectly(const SharedPtr<Stroke>& stroke);
    auto initImg(bool flag) -> bool;

    /**
     * \brief This method is used to get a painter which is already ready for drawing
     *  on the current image in the page.
     *  If, for any reason, an error occurred, the value in the pair will have an error set and the painter return
     *  will be not valid for draw.
     * \param positionAudio Current position of AudioPlayer (if the value is not valid
     *  the strokes with a time bigger that positionAudio will be colored with less alpha.)
     * */
    auto initPainter(AudioPosition positionAudio) -> WPair<Error, WPainter>;

    void decreaseAlfa(const WVector<int> &pos, WPainter *painter, int decrease);

    static auto at_translation(const PointF &point, cint page) -> PointF;
    static auto get_size_area(const WListFast<SharedPtr<Stroke>> &strokes, int from, int to) -> RectF;

public:
    auto getImg() const -> const WPixmap&;

    Page ();
    Page (Page &&other) noexcept;
    Page (const Page &from);
    Page (int count, n_style style);
    ~Page();

    /**
     * \brief This method need to be called in the main function before do anything on a document and
     * all the classed he use.
     * This method require that a instance of Scheduler is present
     * */
    static auto init() -> void;

    auto clearAudio() -> void;

    struct SwapItemFlag {
        enum swapItemFlag {
            Zero = 0x0,
            TriggerView = 0x1 // Passing this value as parameter will trigger the view of the items removed
        };
    };

    auto swap(const WVector<int> & pos, WFlags<SwapItemFlag::swapItemFlag> flag) -> WListFast<SharedPtr<Stroke>>;
    auto swap(int from, int to) -> WListFast<SharedPtr<Stroke>>;
    auto swap(int index, const SharedPtr<Stroke>& newData) -> SharedPtr<Stroke>;

    bool updateFlag(const PointF &FirstPoint, double zoom, double heightView);
    void setVisible(bool vis) const;

    __fast int lengthStroke() const;

    bool isVisible() const;

    /**
     * \param pos to order positions of the stroke to be removed
     * */
    template <class Iter>
    void removeAt(Iter begin, Iter end);

    auto last() const -> const Stroke &;
    auto lastMod() -> Stroke &;

    /**
     *  these 3 functions do not automatically launch
     *  the drawing of the whole sheet, they wait for
     *  the triggerRenderImage to be executed.
    */
    __fast void append(const SharedPtr<Stroke> &stroke);
    __fast void append(SharedPtr<Stroke> &&stroke);
    template <template <class T> class List>
    __fast void append(const List<SharedPtr<Stroke>> &stroke);

    __fast void reserve(int stroke) { _strokeTmp.reserve(stroke); } ;
    __fast auto atStroke(int i) const -> const Stroke&;

    [[deprecated]]
    __fast auto get_stroke_page() const -> const StrokeForPage &; //return the point written by writernote
    __slow void at_draw_page(int IndexPoint, const PointF &translation, PointF &point, double zoom) const;

    auto minHeight() const -> double;
    auto currentHeight() const -> double;
    auto currentWidth() const -> double;

    auto userWrittenSomething() const -> bool;

    [[deprecated]]
    auto move(uint from, uint to) -> void;

    void triggerRenderImage(AudioPosition m_pos_ris, bool all);

    /**
     * \brief Getter function to retrieve the counter of the page
     *  If you have to retrieve the index use \link getIndex
     *  This method is commonly used when drawing stroke, because
     *  we need to shift all the points based on the page in which they are
     *  located
     * \return The counter of the page, the counter start from 1.
     * */
    auto getCount() const -> int;

    /**
     * \brief Getter function to retrieve the index of the page
     *  If you have to retrieve the counter use \link getCounter
     * \return The counter of the page, the counter start from 0.
     * */
    auto getIndex() const -> int;

    void reset();

    /**
     * \brief This method draw all stroke for which their position is contained in 'positions'
     * \param m_pos_ris The position, in seconds, of the audio player (pass -1 in case there is no audio
     *  playing)
     * \param positions The indexes fo the strokes to draw
     */
    void drawStroke(const WVector<int> &positions, AudioPosition m_pos_ris);
    void drawStroke(const Stroke &stroke, AudioPosition m_pos_ris);
    void drawForceColorStroke(const Stroke &stroke, AudioPosition m_pos_ris, const WColor &color, WPainter *painter);
    void drawForceColorStroke(const WVector<int> &pos, AudioPosition m_pos_ris, const WColor &color);

    void removeAndDraw(AudioPosition m_pos_ris, const WVector<int> &pos, const RectF &area);
    void drawIfInside(AudioPosition m_pos_ris, const RectF &area);
    void decreaseAlfa(const WVector<int> &pos, int decrease);

    // TODO: rename to getSizeArea
    auto get_size_area(const WVector<int> &pos) const -> RectF;

    // block for appending
    void lock() const;
    void unlock() const;

    void setCount(int count);

    static auto write (WritableAbstract &writable, const Page &page, bool saveImage = true) -> Error;
    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, Page>;

    //static void copy(const Page &src, Page &dest);
    constexpr static auto getProportion() -> double;
    constexpr static auto getHeight() -> double;
    constexpr static auto getWidth() -> double;
    constexpr static auto getResolutionSize() -> WSize;

    constexpr static auto size() -> PointF;
    constexpr static auto sizePoint() -> PointF;

    constexpr static auto getResolutionWidth() -> double;
    constexpr static auto getResolutionHeight() -> double;

    struct DrawToPageFlag {
        enum drawToPageFlag {
            Zero = 0x0,
            initImage = 0x1 // if we need to init the image in case it's null
        };
    };
    /**
     * \brief This method will draw the stroke own by the page into the pixmap passed as parameter
     * \param indexes The set of position of the stroke to draw into the pixmap passed as parameter
     * \param img The image on which you need to draw
     * \param flag Some flags defined in DrawToPageFlag
     * */
    void drawToImage(const WVector<int> &indexes, WPixmap &img, WFlags<DrawToPageFlag::drawToPageFlag> flag) const;

    auto operator=(const Page &other) noexcept -> Page &;
    auto operator=(Page &&other) noexcept -> Page &;

    auto operator==(const Page &other) const noexcept -> bool;

    auto operator[](int index) -> Stroke& { return *this->_stroke[index]; }

    auto begin() { return _stroke.begin(); }
    auto end() { return _stroke.end(); }
    auto begin() const { return _stroke.begin(); }
    auto end() const { return _stroke.end(); }

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
    friend class PageDrawTask;
    friend void adjustStrokePage(WList<Stroke> &List, int count, Stroke *m_stroke);
    friend class copy;
    friend void actionRubberSingleTotal(struct DataPrivateMuThread *_data);
};

template <template <class T> class List>
void Page::append(const List<SharedPtr<Stroke>> &stroke)
{
    reserve(stroke.size());
    for (const auto& ref: std::as_const(stroke)) {
        append(ref);
    }
}

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

inline PointF Page::at_translation(const PointF &point, int page)
{
    PointF tmp;
    const double ytranslation = double(page) * Page::getHeight();

    if (page == 0) {
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

force_inline void Page::appendDirectly(const SharedPtr<Stroke>& stroke)
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
    return WSize(Page::getResolutionWidth(), Page::getResolutionHeight());
}

constexpr force_inline double Page::getResolutionWidth()
{
    return getWidth() * PROP_RESOLUTION;
}

constexpr force_inline double Page::getResolutionHeight()
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

    _isVisible = WUtils::included(0.0, heightView, minH) or
                 WUtils::included(0.0, heightView, maxH);

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

force_inline auto Page::atStroke(int i) const -> const Stroke &
{
    rep();
    const auto res = this->_stroke.at(i);
    W_ASSERT(res);
    return *res;
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

/* the list should be order */
template <class Iter>
inline void Page::removeAt(Iter begin, Iter end)
{
    W_ASSERT(WAbstractList::isSorted(begin, end));

    _stroke.removeAt(begin, end);
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

inline void Page::append(SharedPtr<Stroke> &&stroke)
{
    W_ASSERT(stroke and not stroke->isEmpty());

    rep();

    this->_strokeTmp.append(std::move(stroke));

    WDebug(pageDebug, this->_count);
}

force_inline void Page::append(const SharedPtr<Stroke> &stroke)
{
    auto copy = stroke;
    return this->append(std::move(copy));
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
        _strokeTmp.append(s->clone());
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
        AudioPosition   m_pos_ris,
        const WColor    &color,
        WPainter        *painter)
{
    const auto needDelete = bool (painter == nullptr);

    if (needDelete) {
        if (initImg(false))
            return this->triggerRenderImage(m_pos_ris, true);

        painter = new WPainterUnsafe();

        if (!painter->begin(&this->_imgDraw))
            std::abort();
    }

    this->drawStroke(*painter, stroke, color);

    W_ASSERT(painter->isActive());

    if (needDelete) {
        painter->end();
        delete painter;
    }
}

force_inline constexpr PointF Page::sizePoint()
{
    return PointF {
            Page::size().x(),
            Page::size().y()
    };
}

inline auto Page::operator==(const Page &other) const noexcept -> bool
{
    const auto cmp = [] (auto& stroke1, auto& stroke2) {
        return static_cast<const Stroke&>(*stroke1) == static_cast<const Stroke&>(*stroke2);
    };

    const auto strokeEquals = _stroke.equals(other._stroke, cmp);
    const auto isVisibleEquals = this->_isVisible == other._isVisible;

    const auto strokeTmpEquals = _strokeTmp.equals(other._strokeTmp, cmp);
    const auto strokeWriternoteEquals = this->_stroke_writernote == other._stroke_writernote;

    return strokeEquals
        and isVisibleEquals
        and strokeTmpEquals
        and strokeWriternoteEquals
        and this->_count == other._count;
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

inline auto Page::load(const VersionFileController &versionController,
                       ReadableAbstract &readable) -> WPair<Error, Page>
{
    Page result;
    bool savedImage;

    if (versionController.getVersionPage() != 0)
        return {Error::makeErrVersion(), result};

    static_assert_type(savedImage, bool);
    static_assert_type(result._count, int);
    static_assert_type(result._isVisible, bool);

    if (auto err = readable.read(savedImage))
        return {err, {}};

    if (auto err = readable.read(result._count))
        return {err, {}};

    if (auto err = readable.read(result._isVisible))
        return {err, {}};

    {
        auto [res, list] = WListFast<SharedPtr<Stroke>>::load (versionController, readable);
        if (res)
            return {res, {}};

        result._stroke = std::move (list);
    }

    {
        auto [res, list] = WListFast<SharedPtr<Stroke>>::load (versionController, readable);
        if (res)
            return {res, {}};
        result._strokeTmp = std::move (list);
    }

    // fino a qua giusto
    {
        auto [res, str] = StrokeForPage::load (versionController, readable);
        if (res)
            return {res, {}};
        result._stroke_writernote = std::move (str);
    }

    if (savedImage) {
        auto [res, img] = WPixmap::load (versionController, readable);
        if (res)
            return {res, {}};
        result._imgDraw = std::move (img);
    }

    return {Error::makeOk(), std::move(result)};
}

// TODO: move into cpp file
inline auto Page::write(WritableAbstract &writable, const Page &page, bool saveImage) -> Error
{
    static_assert_type(saveImage, bool);
    static_assert_type(page._count, int);
    static_assert_type(page._isVisible, bool);

    if (auto err = writable.write(saveImage))
        return err;

    if (auto err = writable.write(page._count))
        return err;

    if (auto err = writable.write(page._isVisible))
        return err;

    if (auto err = WListFast<SharedPtr<Stroke>>::write(writable, page._stroke))
        return err;

    if (auto err = WListFast<SharedPtr<Stroke>>::write(writable, page._strokeTmp))
        return err;

    if (auto err = StrokeForPage::write(writable, page._stroke_writernote))
        return err;

    if (saveImage)
        if (auto err = WPixmap::write(writable, page._imgDraw))
            return err;

    return Error::makeOk();
}

inline auto Page::initPainter(AudioPosition positionAudio) -> WPair<Error, WPainter>
{
    if (initImg(false)) {
        this->triggerRenderImage(positionAudio, true);
        return {Error::makeErrGeneric(), {}};
    }

    WPainterUnsafe painter;
    painter.begin(&_imgDraw);

    return {Error::makeOk(), std::move(painter)};
}
