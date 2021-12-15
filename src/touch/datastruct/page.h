#ifndef PAGE_H
#define PAGE_H

#include <QList>
#include <QPointF>
#include <QDebug>
#include <QImage>
#include "stroke.h"
#include "../../log/log_ui/log_ui.h"
#include "../../utils/common_def.h"

enum n_style: int;

class page
{
private:
    static constexpr uint width = 1666;
    static constexpr double proportion = 1.4141;
    static constexpr uint height = width*proportion; // correct proportions for A4 paper size

    bool IsVisible = true;
    int count;

    QList<stroke> m_stroke;
    QList<stroke> m_stroke_writernote;

    /* after adding data to the list, call triggernewview,
     *  and pass as all false, in this way what is
     * to be drawn will be drawn above the current image, and
     * then strokeTmp will be added to the stroke list
    */
    QList<stroke> strokeTmp;

    void drawNewPage(n_style __style);

    QImage imgDraw;
    void drawEngine(QPainter &painter, QList<stroke> &List, const int m_pos_ris);
    void draw(QPainter &painter, const int m_pos_ris, const bool all);

    void mergeList();

    static point_s at_translation(const point_s &point, int page);

    void AppendDirectly(const stroke &stroke);

public:
    const QImage &getImg() const;

    page(const int count, const n_style style);

    Q_CONSTEXPR static double getProportion();
    Q_CONSTEXPR static double getHeight();
    Q_CONSTEXPR static double getWidth();

    Q_CONSTEXPR static double getResolutionWidth();
    Q_CONSTEXPR static double getResolutionHeigth();

    void updateFlag(const QPointF &FirstPoint, const double zoom, const double heightView);
    void setVisible(const bool vis);

    __slow void at_draw(const uint IndexStroke, const uint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const;

    /* you can access point written by writernote with this funcion */
    __slow void at_draw_page(const uint IndexStroke, const uint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const;

    __fast uint lengthStroke() const;
    __fast uint lengthStrokePage() const; // len point written by writernote

    bool isVisible() const;
    static void copy(const page &src, page &dest);
    void removeAt(const uint i);
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

    void triggerRenderImage(int m_pos_ris, const bool all);

    void reset();
    void allocateStroke(int numAllocation);

    int save(zip_source_t *file) const;
    int load(zip_file_t *file, int ver_stroke, int len_stroke);

    QList<stroke>::const_iterator get_begin() const noexcept;
    QList<stroke>::const_iterator get_end() const noexcept;

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

inline void page::updateFlag(const QPointF &FirstPoint, const double zoom, const double heightView)
{
    static double heightSec;
    static double translation;

    heightSec = double(height)*zoom;
    translation = -FirstPoint.y();

    IsVisible =  ((heightSec*count) >= translation) && ((heightSec*(count-1)) <= translation);
    IsVisible = (IsVisible) ? IsVisible : (count-1)*heightSec <= heightView && (count-1)*heightSec >= translation;
    IsVisible = true;
}

inline void page::setVisible(const bool vis)
{
    this->IsVisible = vis;
}

inline const stroke &page::atStroke(uint i) const
{
    return this->m_stroke.at(i);
}

inline stroke &page::atStrokeMod(const uint i)
{
    return this->m_stroke.operator[](i);
}

inline const stroke &page::atStrokePage(const uint i) const
{
    return this->m_stroke_writernote.at(i);
}

static inline void __at_draw_private(const point_s &from, point_s &to, const double zoom, const QPointF &translation)
{
    memcpy(&to, &from, sizeof(point_s));

    to.m_x *= zoom;
    to.m_y *= zoom;

    to.m_x += translation.x();
    to.m_y += translation.y();
}

inline void page::at_draw(const uint IndexStroke, const uint IndexPoint, const QPointF &translation,
                          point_s &point, const double zoom) const
{
    const stroke &stroke = atStroke(IndexStroke);
    const point_s &__point = stroke.at(IndexPoint);

    __at_draw_private(__point, point, zoom, translation);
}

inline void page::at_draw_page(const uint IndexStroke, const uint IndexPoint, const QPointF &translation, point_s &point, const double zoom) const
{
    const stroke &stroke = this->m_stroke_writernote.at(IndexStroke);
    const point_s &__point = stroke.at(IndexPoint);

    __at_draw_private(__point, point, zoom, translation);
}

inline uint page::lengthStroke() const
{
    return m_stroke.length();
}

inline uint page::lengthStrokePage() const
{
    return this->m_stroke_writernote.length();
}

inline bool page::isVisible() const
{
    return this->IsVisible;
}

inline void page::copy(const page &src, page &dest){
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

inline void page::removeAt(const uint i)
{
    this->m_stroke.removeAt(i);
}

inline int page::maxId() const
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

inline const stroke &page::last() const
{
    return this->m_stroke.last();
}

inline stroke &page::lastMod()
{
    return this->m_stroke.operator[](this->lengthStroke() - 1);
}

inline void page::append(const stroke &strokeAppend)
{
    int lastNewIndex = strokeTmp.length();
    this->strokeTmp.append(strokeAppend);
    //this->strokeTmp.operator[](lastNewIndex) = strokeAppend;

    /* they will be automatically removed when
     * the project is compiled in release mode */
    Q_ASSERT(strokeTmp.at(lastNewIndex).length() == strokeAppend.length());
    Q_ASSERT(strokeTmp.at(lastNewIndex).getId()  == strokeAppend.getId());
}

inline void page::appendToTheTop(const stroke &stroke)
{
    m_stroke.insert(0, stroke);
}

inline double page::minHeight() const
{
    return (this->count-1)*this->height;
}

#endif // PAGE_H
