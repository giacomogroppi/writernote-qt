#pragma once

#include <QVector>
#include <QList>
#include <QPointF>
#include <QDebug>
#include "core/WMutex.h"
#include "core/WZipWriterSingle.h"
#include "core/core.h"
#include "touch/dataTouch/stroke/Stroke.h"
#include "utils/common_def.h"
#include "utils/WCommonScript.h"
#include "core/WZipReaderSingle.h"
#include <pthread.h>
#include "core/WImage.h"
#include "touch/dataTouch/stroke/StrokeForPage.h"


#define COLOR_NULL QColor::fromRgb(255, 255, 255, 255)
#define Define_PEN(pen) QPen pen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
#define TEMP_COLOR QColor::fromRgb(105, 105, 105, 255)
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

#define __is_ok_count() W_ASSERT(this->_count > 0);

#define SetRenderPainter(painter) painter.setRenderHint(QPainter::Antialiasing, true);

#define Define_PAINTER_p(painter, ___img)               \
    QPainter painter;                                   \
    if(!painter.begin(&___img)) {                       \
        if constexpr(WCommonScript::debug_enable()){    \
            std::abort();                               \
        }                                               \
    };                                                  \
    SetRenderPainter(painter);

#define Define_PAINTER(painter) Define_PAINTER_p(painter, _imgDraw)

#define End_painter(painter) if(!painter.end()) { if(WCommonScript::debug_enable()){ std::abort(); }  };


enum n_style: int;

void adjustStrokePage(const QList<Stroke> &List, int count, Stroke *m_stroke);

constexpr bool debugPage = false;

class Page
{
private:
    static constexpr uint width = 1666;
    static constexpr double proportion = 1.4141;
    static constexpr uint height = width * proportion; // correct proportions for A4 paper size

    WMutex _img, _append_load;
    bool                _IsVisible = true;
    int                 _count;
    QList<Stroke *>     _stroke;
    StrokeForPage       _stroke_writernote;

    /* after adding data to the list, call triggernewimage,
     *  and pass as all false, in this way what is
     * to be drawn will be drawn above the current image, and
     * then strokeTmp will be added to the stroke list
    */
    QVector<Stroke *>   _strokeTmp;
    WImage              _imgDraw;

    void drawNewPage(n_style __style);
    
    void drawEngine(QPainter &painter, QList<Stroke> &List, int m_pos_ris, bool *changeSomething, cbool use_multi_thread);
    void draw(QPainter &painter, int m_pos_ris, bool all);
    void drawStroke(QPainter &painter, const Stroke &stroke, QPen &pen, const QColor &color) const;

    void mergeList();    

    void AppendDirectly(Stroke *stroke);
    bool initImg(bool flag);

    void decreseAlfa(const QVector<int> &pos, QPainter *painter, int decrese);

    static point_s at_translation(const point_s &point, cint page);
    static QRect get_size_area(const QList<Stroke *> & item, int from, int to);

public:
    const WImage &getImg() const;

    Page();
    Page(const Page &page);
    Page(const int count, const n_style style);
    ~Page();

#define PAGE_SWAP_TRIGGER_VIEW BIT(1)
    void swap(QList<Stroke *> & stroke, const QVector<int> & pos, int flag);
    void swap(QList<Stroke *> & stroke, int from, int to);

    bool updateFlag(const QPointF &FirstPoint, const double zoom, const double heightView);
    void setVisible(cbool vis) const;
    size_t get_size_in_file(cbool saveImg) const;

    __fast int lengthStroke() const;

    bool isVisible() const;

    void removeAt(const QVector<int> & pos);
    void removeAt(cint i);

    const Stroke & last() const;
    Stroke &lastMod();

    /*
     *  these 3 functions do not automatically launch
     *  the drawing of the whole sheet, they wait for
     *  the triggerRenderImage to be executed.
    */
    __fast void append(Stroke *stroke);
    __fast void append(const QList<Stroke *> & stroke);

    __fast const Stroke       & atStroke(const uint i) const;
    __fast Stroke             & atStrokeMod(const uint i);

    __fast const StrokeForPage &get_stroke_page() const; //return the point written by writernote
    __slow void at_draw_page(cint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const;

    double minHeight() const;
    double currentHeight() const;
    double currentWidth() const;

    void changeId(const uint i, const int newId);
    bool userWrittenSomething() const;
    void move(const uint from, const uint to);

    void triggerRenderImage(int m_pos_ris, bool all);

    int getCount() const;

    void reset();
    void allocateStroke(int numAllocation);

    int save(WZipWriterSingle &writer, cbool saveImg) const;

    int load(WZipReaderSingle &reader, int ver_stroke);

    void drawStroke(const Stroke &stroke, int m_pos_ris);
    void drawForceColorStroke(const Stroke &stroke, cint m_pos_ris, const QColor &color, QPainter *painter);
    void drawForceColorStroke(const QVector<int> &pos, int m_pos_ris, const QColor &color);

    void removeAndDraw(int m_pos_ris, const QVector<int> &pos, const QRectF &area);
    void drawIfInside(int m_pos_ris, const QRectF &area);
    void drawSquare(const QRect &rect);
    void decreseAlfa(const QVector<int> &pos, int decrese);

    QRect get_size_area(const QVector<int> &pos) const;

    // block for appending
    void lock() const;
    void unlock() const;

    void setCount(int count);

    static void copy(const Page &src, Page &dest);
    Q_CONSTEXPR static double getProportion();
    Q_CONSTEXPR static double getHeight();
    Q_CONSTEXPR static double getWidth();
    Q_CONSTEXPR static QSize getResolutionSize();

    Q_CONSTEXPR static double getResolutionWidth();
    Q_CONSTEXPR static double getResolutionHeigth();

#define DR_IMG_INIT_IMG BIT(1) // init the image with a image trasparent
    void drawToImage(const QVector<int> &index, WImage &img, cint flag) const;

    Page &operator=(const Page &other);

    friend class StrokeLine;
    friend class Stroke;
    friend class page_file;
    friend class stroke_drawer;
    friend class datastruct;
    friend class xmlstruct;
    friend class RubberMethod;
    friend void * __page_load(void *);
    friend void adjustStrokePage(QList<Stroke> &List, int count, Stroke *m_stroke);
    friend class copy;
    friend void actionRubberSingleTotal(struct DataPrivateMuThread *_data);
};

force_inline void Page::unlock() const
{
    auto &m = (WMutex &) _img;
    m.unlock();
}

force_inline void Page::lock() const
{
    auto &m = (WMutex &)(_img);
    m.lock();
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
    this->_IsVisible = true;
    this->_count = -1;
    this->_strokeTmp.clear();
    this->_imgDraw = WImage();
}

inline point_s Page::at_translation(const point_s &point, cint page)
{
    point_s tmp;
    const double ytranslation = double(page) * Page::getHeight();

    if(unlikely(!page)){
        return point;
    }

    memcpy(&tmp, &point, sizeof(tmp));
    tmp.ry() -= ytranslation;
    return tmp;
}

force_inline void Page::AppendDirectly(Stroke *stroke)
{
    this->_stroke.append(stroke);
}

force_inline const WImage &Page::getImg() const
{
    return this->_imgDraw;
}

Q_CONSTEXPR force_inline double Page::getProportion()
{
    return proportion;
}

Q_CONSTEXPR force_inline double Page::getHeight()
{
    return height;
}

Q_CONSTEXPR force_inline double Page::getWidth()
{
    return width;
}

Q_CONSTEXPR force_inline QSize Page::getResolutionSize()
{
    return QSize(Page::getResolutionWidth(), Page::getResolutionHeigth());
}

#define PROP_RESOLUTION (2.)
Q_CONSTEXPR force_inline double Page::getResolutionWidth()
{
    return getWidth() * PROP_RESOLUTION;
}

Q_CONSTEXPR force_inline double Page::getResolutionHeigth()
{
    return getHeight() * PROP_RESOLUTION;
}

force_inline bool Page::updateFlag(
        const QPointF   &FirstPoint,
        cdouble         zoom,
        cdouble         heightView)
{
    double heightSec;

    W_ASSERT(zoom >= 0);
    //W_ASSERT(FirstPoint.x() <= 0.0 && FirstPoint.y() <= 0.0);
    W_ASSERT(heightView >= 0);

    heightSec = Page::getHeight() * zoom;

    cdouble minH = heightSec * double(_count - 1) / zoom + FirstPoint.y();
    cdouble maxH = heightSec * double(_count)     / zoom + FirstPoint.y();

    if(likely(heightView <= Page::getHeight() * zoom)){
        // if the page is not fully visible in a window

        _IsVisible = discordant(maxH, minH);

        if(_IsVisible)
            goto ret;
    }

    _IsVisible =    WCommonScript::included(0.0, heightView, minH) or
                    WCommonScript::included(0.0, heightView, maxH);

ret:
    WDebug(debugPage, "count"   << _count
             << "minH"          << minH
             << "heightView"    << heightView
             << "maxH"          << maxH
             << "zoom"          << zoom
             << _IsVisible);

    return _IsVisible;
}

force_inline void Page::setVisible(cbool vis) const
{
    bool &IsVisible = (bool &)_IsVisible;
    IsVisible = vis;
}

force_inline const Stroke &Page::atStroke(uint i) const
{
    __is_ok_count();
    auto *res = this->_stroke.at(i);
    W_ASSERT(res);
    return *res;
}

force_inline Stroke &Page::atStrokeMod(const uint i)
{
    __is_ok_count();
    auto *res = this->_stroke.operator[](i);
    W_ASSERT(res);
    return *res;
}

force_inline const StrokeForPage &Page::get_stroke_page() const
{
    __is_ok_count();
    return this->_stroke_writernote;
}

static force_inline void __at_draw_private(const point_s &from, point_s &to, const double zoom, const QPointF &translation)
{
    memcpy(&to, &from, sizeof(from));

    to *= zoom;
    to += translation;
}

inline void Page::at_draw_page(
        cint            IndexPoint,
        const QPointF   &translation,
        point_s         &point,
        const double    zoom) const
{
    const auto &stroke = get_stroke_page();
    const point_s &p = stroke._data._point.at(IndexPoint);

    __at_draw_private(p, point, zoom, translation);
}

force_inline int Page::lengthStroke() const
{
    return _stroke.length();
}

force_inline bool Page::isVisible() const
{
    return this->_IsVisible;
}

inline void Page::copy(
        const Page  &src,
        Page        &dest)
{
    //int counterStroke, lenStroke;
    //lenStroke = src.lengthStroke();
    dest.reset();

    //dest.allocateStroke(src.lengthStroke());

    dest._stroke                    = src._stroke;
    dest._stroke_writernote         = src._stroke_writernote;
    dest._strokeTmp                 = src._strokeTmp;
    dest._imgDraw                   = src._imgDraw;
    dest._IsVisible                 = src._IsVisible;
    dest._count                     = src._count;
}

force_inline void Page::removeAt(cint i)
{
    W_ASSERT(!(i < 0 || i >= _stroke.size()));

    auto *res = this->_stroke.takeAt(i);
    W_ASSERT(res);
    delete res;
}

force_inline const Stroke &Page::last() const
{
    __is_ok_count();
    return *this->_stroke.last();
}

inline Stroke &Page::lastMod()
{
    __is_ok_count();
    return *this->_stroke.operator[](this->lengthStroke() - 1);
}

force_inline void Page::append(Stroke *strokeAppend)
{
    DO_IF_DEBUG(
    int not_used lastNewIndex = _strokeTmp.length();
    );

    W_ASSERT(strokeAppend);

    __is_ok_count();
    W_ASSERT(!strokeAppend->isEmpty());

    this->_strokeTmp.append(strokeAppend);
}

force_inline double Page::minHeight() const
{
    return (this->_count - 1) * Page::height;
}

force_inline Page::Page(const Page &from)
{
    Page::copy(from, *this);
}

force_inline Page::~Page()
{
}

inline Page &Page::operator=(const Page &other)
{
    if(this == &other){
        return *this;
    }

    Page::copy(other, *this);
    return *this;
}

force_inline void Page::drawForceColorStroke(
        const Stroke    &stroke,
        cint            m_pos_ris,
        const QColor    &color,
        QPainter        *painter)
{
    Define_PEN(pen);
    const auto needDelete = (bool) (!painter);

    if(needDelete){
        if(unlikely(initImg(false)))
            return this->triggerRenderImage(m_pos_ris, true);

        WNew(painter, QPainter, ());

        if(!painter->begin(&this->_imgDraw))
            std::abort();
        core::painter_set_antialiasing(*painter);
    }

    this->drawStroke(*painter, stroke, pen, color);
    W_ASSERT(painter->isActive());

    if(needDelete){
        painter->end();
        WDelete(painter);
    }
}
