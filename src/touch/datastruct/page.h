#ifndef PAGE_H
#define PAGE_H

#include <QList>
#include <QPointF>
#include <QDebug>
#include <QImage>
#include "stroke.h"
#include "src/log/log_ui/log_ui.h"
#include "src/utils/common_def.h"
#include "src/utils/common_script.h"

#define COLOR_NULL QColor::fromRgb(255, 255, 255, 255)

enum n_style: int;

class page
{
private:
    static constexpr uint width = 1666;
    static constexpr double proportion = 1.4141;
    static constexpr uint height = width*proportion; // correct proportions for A4 paper size

#define FLAG_PAGE_ORDERED (1 << 0) // if indicates whether the list of strokes is sort by index
    int flag = 0;

    bool IsVisible = true;
    int count;

    QList<stroke> m_stroke;
    QList<stroke> m_stroke_writernote;

    /* after adding data to the list, call triggernewimage,
     *  and pass as all false, in this way what is
     * to be drawn will be drawn above the current image, and
     * then strokeTmp will be added to the stroke list
    */
    QList<stroke> strokeTmp;

    void drawNewPage(n_style __style);

    QImage imgDraw;
    void drawEngine(QPainter &painter, QList<stroke> &List, int m_pos_ris);
    void draw(QPainter &painter, int m_pos_ris, bool all);
    void drawStroke(QPainter &painter, const stroke &stroke, QPen &pen, const QColor &color);

    void mergeList();

    static point_s at_translation(const point_s &point, int page);

    void AppendDirectly(const stroke &stroke);
    bool initImg(bool flag);

#ifdef DEBUGINFO
    void ctrlOrdered() const;
#endif
    bool isOrdered() const;
    void setOrdered();
    void setNotOrdered();

public:
    const QImage &getImg() const;

    page(const int count, const n_style style);

    Q_CONSTEXPR static double getProportion();
    Q_CONSTEXPR static double getHeight();
    Q_CONSTEXPR static double getWidth();

    Q_CONSTEXPR static double getResolutionWidth();
    Q_CONSTEXPR static double getResolutionHeigth();

    bool updateFlag(const QPointF &FirstPoint, const double zoom, const double heightView);
    void setVisible(cbool vis);

    __slow void at_draw(const uint IndexStroke, const uint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const;

    /* you can access point written by writernote with this funcion */
    __slow void at_draw_page(const uint IndexStroke, const uint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const;

    __fast uint lengthStroke() const;
    __fast uint lengthStrokePage() const; // len point written by writernote

    bool isVisible() const;
    static void copy(const page &src, page &dest);
    void removeAt(cuint i);
    int maxId() const;

    const stroke & last() const;
    stroke &lastMod();

    /*
     *  these 3 functions do not automatically launch
     *  the drawing of the whole sheet, they wait for
     *  the triggerRenderImage to be executed.
    */
    __fast void append(const stroke &point);

    __slow void appendToTheTop(const stroke &point);

    __fast const stroke       & atStroke(const uint i) const;
    __fast stroke             & atStrokeMod(const uint i);

    __fast const stroke       & atStrokePage(const uint i) const; //return the point written by writernote

    double minHeight() const;
    double currentHeight() const;
    double currentWidth() const;

    void changeCounter(const int newPage);
    void changeId(const uint i, const int newId);
    bool userWrittenSomething() const;
    void move(const uint from, const uint to);

    void triggerRenderImage(int m_pos_ris, bool all);

    void reset();
    void allocateStroke(int numAllocation);

    int save(zip_source_t *file) const;
    int load(zip_file_t *file, int ver_stroke, int len_stroke);

    QList<stroke>::const_iterator get_begin() const noexcept;
    QList<stroke>::const_iterator get_end() const noexcept;

    void drawStroke(const stroke &stroke, int m_pos_ris);
    void drawForceColorStroke(const stroke &stroke, int m_pos_ris, const QColor &color);
    void drawForceColor(int m_pos_ris, const QList<int> &id, const QColor &color);
    int removeAndDraw(int m_pos_ris, const QList<int> &id, const QRectF &area);

    friend class stroke;
    friend class datastruct;
    friend class xmlstruct;
};

inline double page::currentHeight() const
{
    return count*page::getHeight();
}

inline double page::currentWidth() const
{
    return page::getWidth();
}

inline void page::move(const uint from, const uint to)
{
    this->m_stroke.move(from, to);
}

inline void page::reset()
{
    this->m_stroke.clear();
}

inline QList<stroke>::const_iterator page::get_begin() const noexcept
{
    return m_stroke.begin();
}

inline QList<stroke>::const_iterator page::get_end() const noexcept
{
    return m_stroke.end();
}

inline point_s page::at_translation(const point_s &point, int page)
{
    point_s tmp;
    const double ytranslation = double(page) * page::getHeight();

    memcpy(&tmp, &point, sizeof(tmp));
    tmp.m_y -= ytranslation;
    return tmp;
}

Q_ALWAYS_INLINE void page::AppendDirectly(const stroke &stroke)
{
    this->m_stroke.append(stroke);
}

#if defined(DEBUGINFO)
inline void page::ctrlOrdered() const
{
    const auto func = IS_ORDER_WITH_FUNCTION(m_stroke, getId);

    if(isOrdered() && !func.operator()(m_stroke))
        std::abort();
}
#endif

inline void page::setOrdered()
{
    this->flag |= FLAG_PAGE_ORDERED;
}

inline void page::setNotOrdered()
{
    this->flag &= ~(FLAG_PAGE_ORDERED);
}

Q_ALWAYS_INLINE const QImage &page::getImg() const
{
    return this->imgDraw;
}

Q_CONSTEXPR Q_ALWAYS_INLINE double page::getProportion()
{
    return proportion;
}

Q_CONSTEXPR Q_ALWAYS_INLINE double page::getHeight(){
    return height;
}

Q_CONSTEXPR Q_ALWAYS_INLINE double page::getWidth()
{
    return width;
}

#define PROP_RESOLUTION (double(2))
Q_CONSTEXPR Q_ALWAYS_INLINE double page::getResolutionWidth()
{
    return getWidth() * PROP_RESOLUTION;
}

Q_CONSTEXPR Q_ALWAYS_INLINE double page::getResolutionHeigth()
{
    return getHeight() * PROP_RESOLUTION;
}

Q_ALWAYS_INLINE bool page::updateFlag(
        const QPointF   &FirstPoint,
        cdouble         zoom,
        cdouble         heightView)
{
    double heightSec;

    heightSec = double(page::getHeight()) * zoom;

    cdouble minH = heightSec * double(count - 1) / zoom + FirstPoint.y();
    cdouble maxH = heightSec * double(count)     / zoom + FirstPoint.y();

    if(heightView < page::getHeight() * zoom){
        // if the page is not fully visible in a window

        IsVisible = (maxH)
                *   (minH) <= 0.0;

        if(IsVisible)
            goto ret;
    }

    IsVisible = included(0.0, heightView, minH) || included(0.0, heightView, maxH);

ret:
    qDebug() << "count" << count
             << "minH"<< minH
             << "heightView" << heightView
             << "maxH" << maxH
             << "zoom" << zoom
             << IsVisible;

    return IsVisible;
}

Q_ALWAYS_INLINE void page::setVisible(cbool vis)
{
    this->IsVisible = vis;
}

Q_ALWAYS_INLINE const stroke &page::atStroke(uint i) const
{
    return this->m_stroke.at(i);
}

Q_ALWAYS_INLINE stroke &page::atStrokeMod(const uint i)
{
    return this->m_stroke.operator[](i);
}

Q_ALWAYS_INLINE const stroke &page::atStrokePage(const uint i) const
{
    return this->m_stroke_writernote.at(i);
}

static Q_ALWAYS_INLINE void __at_draw_private(const point_s &from, point_s &to, const double zoom, const QPointF &translation)
{
    memcpy(&to, &from, sizeof(point_s));

    to.m_x *= zoom;
    to.m_y *= zoom;

    to.m_x += translation.x();
    to.m_y += translation.y();
}

Q_ALWAYS_INLINE void page::at_draw(const uint IndexStroke, const uint IndexPoint, const QPointF &translation,
                          point_s &point, const double zoom) const
{
    const stroke &stroke = atStroke(IndexStroke);
    const point_s &__point = stroke.at(IndexPoint);

    __at_draw_private(__point, point, zoom, translation);
}

Q_ALWAYS_INLINE void page::at_draw_page(const uint IndexStroke, const uint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const
{
    const stroke &stroke = this->m_stroke_writernote.at(IndexStroke);
    const point_s &__point = stroke.at(IndexPoint);

    __at_draw_private(__point, point, zoom, translation);
}

Q_ALWAYS_INLINE uint page::lengthStroke() const
{
    return m_stroke.length();
}

Q_ALWAYS_INLINE uint page::lengthStrokePage() const
{
    return this->m_stroke_writernote.length();
}

Q_ALWAYS_INLINE bool page::isVisible() const
{
    return this->IsVisible;
}

Q_ALWAYS_INLINE void page::copy(const page &src, page &dest){
    int counterStroke, lenStroke;
    lenStroke = src.lengthStroke();
    dest.reset();

    dest.allocateStroke(src.lengthStroke());

    for(counterStroke = 0; counterStroke < lenStroke; counterStroke ++){
        dest.atStrokeMod(counterStroke) = src.atStroke(counterStroke);
    }

    dest.imgDraw = src.imgDraw;
    dest.IsVisible = src.IsVisible;
    dest.count = src.count;
}

Q_ALWAYS_INLINE void page::removeAt(const uint i)
{
    this->m_stroke.removeAt(i);
}

Q_ALWAYS_INLINE int page::maxId() const
{
    uint i;
    const uint len = lengthStroke();
    int id;

    id = 0;

    for(i = 0; i < len; i++){
        const stroke &stroke = atStroke(i);

        if(stroke.getId() > id)
            id = stroke.getId();
    }

    return id;
}

Q_ALWAYS_INLINE const stroke &page::last() const
{
    return this->m_stroke.last();
}

Q_ALWAYS_INLINE stroke &page::lastMod()
{
    return this->m_stroke.operator[](this->lengthStroke() - 1);
}

Q_ALWAYS_INLINE void page::append(const stroke &strokeAppend)
{
    int lastNewIndex = strokeTmp.length();
    this->strokeTmp.append(strokeAppend);
    //this->strokeTmp.operator[](lastNewIndex) = strokeAppend;

    /* they will be automatically removed when
     * the project is compiled in release mode */
    Q_ASSERT(strokeTmp.at(lastNewIndex).length() == strokeAppend.length());
    Q_ASSERT(strokeTmp.at(lastNewIndex).getId()  == strokeAppend.getId());
}

Q_ALWAYS_INLINE void page::appendToTheTop(const stroke &stroke)
{
    m_stroke.insert(0, stroke);
}

Q_ALWAYS_INLINE double page::minHeight() const
{
    return (this->count-1)*this->height;
}

#endif // PAGE_H
