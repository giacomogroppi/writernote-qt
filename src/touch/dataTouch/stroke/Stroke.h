#pragma once

#include <QList>
#include <QDebug>
#include <QRect>
#include <QPainterPath>
#include "touch/dataTouch/point.h"
#include "utils/common_def.h"
#include "utils/common_script.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "core/WLine.h"
#include "stroke_complex_data.h"
#include "touch/object_finder/model/model.h"
#include "testing/memtest.h"
#include "core/WZipWriterSingle.h"
#include "core/WZipReaderSingle.h"
#include "core/WImage.h"
#include "StrokeProp.h"

struct metadata_stroke{
    int posizione_audio;
    struct colore_s color;
};

class Stroke : public StrokeProp
{
private:
    QList<point_s> _point;
    QList<pressure_t> _pressure;

    struct metadata_stroke _metadata;

    QRect _biggerData;

    enum flag_status : unsigned char{
        UPDATE_BIGGER_DATA = BIT(1),
        UPDATE_PRESSURE = BIT(2)
    };

    unsigned char _flag;

    [[nodiscard]] bool isPressureVal() const;
    [[nodiscard]] bool needToUpdatePressure() const;
    [[nodiscard]] bool needToUpdateBiggerData() const;

    void *_complex = nullptr;
    void setFlag(unsigned char type, bool value) const;

    static_assert(sizeof(_flag) * 8 >= 4 );

    void updateFlagPressure() const;

    void modify();

    int save(WZipWriterSingle &file) const;

    int load(WZipReaderSingle &reader, int version);

public:
    Stroke();
    Stroke(const Stroke &data);
    ~Stroke();

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
    [[nodiscard]] int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const;

    void __setPressureFirstPoint(pressure_t pressure);

    [[nodiscard]] pressure_t getPressure(int index) const;
    [[nodiscard]] pressure_t getPressure() const;
    [[nodiscard]] QColor getColor(double division) const;

    [[nodiscard]] const point_s   &at(int index) const;
    point_s         &at_mod(int index);

#   define stroke_append_default -1.
    void append(const point_s &point, pressure_t pressure);
    void setMetadata(int posizione_audio, const colore_s &color);
    void setMetadata(const metadata_stroke &metadata);
    void setPositioneAudio(int m_pos_ris);
    [[nodiscard]] size_t createControll() const;
    int removeAt(int index);

    /* if you pass from = 0, to = 2 
     * will eliminate points with index 0, 1, 2
    */
    int removeAt(int from, int to); 

    [[nodiscard]] int getPosizioneAudio() const;
    [[nodiscard]] QRect getFirstAndLast() const;
    [[nodiscard]] QRect getBiggerPointInStroke() const;
    [[nodiscard]] bool isInside(const QRectF &rect) const;

    void clearAudio();

    [[nodiscard]] int length() const;
    [[nodiscard]] const struct metadata_stroke &getMetadata() const;
    [[nodiscard]] bool constantPressure() const;

    [[nodiscard]] bool is_highlighter() const;
    [[nodiscard]] uchar get_alfa() const;
    [[nodiscard]] size_t getSizeInMemory() const;
    [[nodiscard]] size_t getSizeInFile() const;
    void decreasePrecision();
    void setAlfaColor(uchar alfa);
    __slow void at_translation(double zoom, point_s &point, int indexPoint, const QPointF &translation) const;

    void setColor(const QColor &color);
    void setColor(const colore_s &color);
    /* this function physically adds the x and y value of the point to all of its points. */
    void movePoint(const QPointF &translation);

    void reset();
    Stroke &operator=(const Stroke &other);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] const point_s &last() const;

    void scale(const QPointF &offset);
    void set_complex(int new_prop, void *new_data);
    [[nodiscard]] const void *get_complex_data() const { return _complex; };

    [[nodiscard]] auto constBegin() const { return _point.begin(); }
    [[nodiscard]] auto constEnd() const { return _point.end(); }

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    friend class Page;
    friend class xmlstruct;
    friend class stroke_drawer;
    friend class stroke_file;
    friend class page_file;
    friend void stroke_complex_adjust(Stroke *stroke, cdouble zoom);

protected:

    void preappend(int l);

    /** T should be a QList, QVector, WList,  .... */
    template <class T>
    [[nodiscard]] static QRect getBiggerPointInStroke(T begin, T end, const Stroke &s);
};

force_inline void Stroke::set_complex(int new_prop, void *new_data)
{
    if(_complex){
        WFree(_complex);
    }

    setProp(new_prop);
    _complex = new_data;

    W_ASSERT(_pressure.isEmpty());
    W_ASSERT(_point.isEmpty());
}

force_inline bool Stroke::isPressureVal() const
{
    return _pressure.length() == 1 && _point.length() > 1;
}

force_inline bool Stroke::needToUpdatePressure() const
{
    return _flag & UPDATE_PRESSURE;
}

force_inline bool Stroke::needToUpdateBiggerData() const
{
    return _flag & UPDATE_BIGGER_DATA;
}

force_inline void Stroke::setFlag(unsigned char type, bool value) const
{
    uchar &__flag = (uchar &) _flag;
    if(value){
        __flag |= type;
    }else{
        __flag &= ~type;
    }
}

inline void Stroke::updateFlagPressure() const
{
    constexpr const auto typeUpdate = UPDATE_PRESSURE;
    int len, i;
    pressure_t press;
    auto &_press = (QList<pressure_t> &)_pressure;

    setFlag(typeUpdate, false);

    len = this->length();

    if(unlikely(len < 2 && _press.length() == 1)){
        // lo stroke era _press const ma gli abbiamo
        // tolto troppi punti, quindi non possiamo più disegnarlo
        // con il QPainterPath
        int i;
        const auto press = _press.at(0);

        for(i = 1; i < len; i++){
            _press.append(press);
        }

        W_ASSERT(_press.length() == _point.length());
    }

    if(unlikely(len > 1 && _press.length() == 1)){
        return;
    }

    W_ASSERT(_pressure.length() == _point.length());

    if(unlikely(len < 3)){
        /* if we have less than 3 points we
         * cannot create a qpainterpath, so
         * we have to draw the stroke point
         * by point.
        */
        return;
    }

    press = _pressure.at(0);

    for(i = 0; i < len; i++){
        if(_pressure.at(i) != press){
            return;
        }
    }

    _press.clear();
    _press.append(press);
}

/* call this function when modify the stroke */
force_inline void Stroke::modify()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PRESSURE;

    W_ASSERT(this->needToUpdateBiggerData());
    W_ASSERT(this->needToUpdatePressure());
}

force_inline Stroke::~Stroke()
{
    this->reset();
}

inline pressure_t Stroke::getPressure(int index) const
{
    int real_index = 0;

    W_ASSERT(is_normal());

    if(_pressure.length() > 1){
        real_index = index;
    }

    return _pressure.at(real_index);
}

/* call this function only when constantPressureVal is 1 */
inline pressure_t Stroke::getPressure() const
{
    W_ASSERT(this->is_normal());
    return _pressure.at(0);
}

inline QColor Stroke::getColor(const double division = 1.) const
{
    return _metadata.color.toQColor(division);
}

inline const point_s &Stroke::at(const int index) const
{
    W_ASSERT(is_normal());
    return _point.at(index);
}

inline point_s &Stroke::at_mod(const int index)
{
    W_ASSERT(is_normal());
    this->modify();
    return _point.operator[](index);
}

inline void Stroke::append(const point_s &point, pressure_t pressure)
{
    /**
     * Appende sempre la pressione del punto, la prima volta che viene
     * aggiornato il flag deciderà se cancellare o meno la list delle
     * pressioni, in caso siano tutte uguali
    */

    if(unlikely(!this->is_normal())){
        W_ASSERT(_point.isEmpty());
        W_ASSERT(_pressure.isEmpty());

        stroke_complex_append(this, point.toQPointF(1.));
        return;
    }

    _point.append(point);
    _pressure.append(pressure);

    this->modify();
}

inline void Stroke::setMetadata(const metadata_stroke &metadata)
{
    memcpy(&_metadata, &metadata, sizeof(_metadata));
}

inline void Stroke::setPositioneAudio(const int m_pos_ris)
{
    _metadata.posizione_audio = m_pos_ris;
}

/* return 1 if the stroke need to be delete */
inline int Stroke::removeAt(int index)
{
    int len = length();
    
    Q_ASSERT(index < len);

    if(this->_pressure.length() > 1){
        W_ASSERT(_pressure.length() == _point.length());
        _pressure.removeAt(index);
    }

    _point.removeAt(index);
    this->modify();

    return len < 2;
}

inline int Stroke::removeAt(int from, int to)
{
    constexpr auto not_used debug_remove_at = false;

    W_ASSERT(to < length());
    W_ASSERT(from >= 0);

    WDebug(debug_remove_at, "from to" << from << to);

    for(; from <= to; to --){
        removeAt(from);
    }
    
    return 0;
}

inline int Stroke::getPosizioneAudio() const
{
    return _metadata.posizione_audio;
}

force_inline QRect Stroke::getFirstAndLast() const
{
    const auto &first = at(0);
    const auto &last  = at(length() - 1);
    return QRect(first.toQPointF(1.).toPoint(),
                 last.toQPointF (1.).toPoint());
}

template<class T>
inline QRect Stroke::getBiggerPointInStroke(T begin,
                                            T end,
                                            const Stroke &s)
{
    QRect biggerData;

    if(unlikely(s.is_complex())){
        biggerData = stroke_complex_bigger_data(&s);
        return biggerData;
    }

    if(unlikely(begin == end)){
        WWarning("Warning: Stroke empty");
        return QRect(0, 0, 0, 0);
    }

    QPoint topLeft = begin->toQPointF(1.).toPoint();
    QPoint bottomRight = topLeft;

    for(; begin != end; begin ++){
        const point_s &point = *begin;

        if(topLeft.x() > point.x())
            topLeft.setX(static_cast<int>(
                        point.x()
                    ));

        if(topLeft.y() > point.y())
            topLeft.setY(static_cast<int>(
                        point.y()
                    ));

        if(bottomRight.x() < point.x())
            bottomRight.setX(static_cast<int>(
                        point.x())
                    );

        if(bottomRight.y() < point.y())
            bottomRight.setY(static_cast<int>(
                        point.y())
                    );
    }

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());

    biggerData = QRect(topLeft, bottomRight);

    return biggerData;
}


/**
 * after append data we need to call this funcion to update
 *
 * the function is defined as const because the only elements
 * we change are stroke metadata. If we removed the constant
 * assignment every time we call this function, it would
 * throw the copy of all the strokes of the page for the
 * redoundo class.
 * We would increase the space occupied, the processing time,
 * as we would need to recalculate the data for each list that
 * has this stroke.
*/
inline QRect Stroke::getBiggerPointInStroke() const
{
    QRect &__biggerData = (QRect &) _biggerData;
    int count;
    QList<point_s>::const_iterator b = this->_point.constBegin();
    QList<point_s>::const_iterator e = this->_point.constEnd();

    if (likely(!needToUpdateBiggerData())) {
        return _biggerData;
    }

    __biggerData = Stroke::getBiggerPointInStroke(b, e, *this);

    setFlag(UPDATE_BIGGER_DATA, false);

    return __biggerData;
}

/* you have to pass a rectangle already
 * translated with respect to the translation
 * point in datastruct, and already
 * multiplied by the zoom. */
inline bool Stroke::isInside(const QRectF &rect) const
{
    int i;

    {
        const auto &area = this->getBiggerPointInStroke();
        if(!area.intersects(rect.toRect())){
            return false;
        }
    }

    if (unlikely(this->is_complex())) {
        return stroke_complex_is_inside(this, rect, 0.);
    }

    for(const auto &ref : qAsConst(_point)){
        if (datastruct_isinside(rect, ref))
            return true;
    }

    return false;
}

inline void Stroke::clearAudio()
{
    _metadata.posizione_audio = -1;
}

force_inline int Stroke::length() const
{
    W_ASSERT(this->is_normal());
    return _point.length();
}

inline const metadata_stroke &Stroke::getMetadata() const
{
    return _metadata;
}

force_inline bool Stroke::constantPressure() const
{
    if(unlikely(needToUpdatePressure()))
        this->updateFlagPressure();

    return isPressureVal();
}

force_inline bool Stroke::is_highlighter() const
{
    return this->get_alfa() < 255;
}

force_inline uchar Stroke::get_alfa() const
{
    return _metadata.color.colore[3];
}

inline void Stroke::setAlfaColor(const uchar alfa)
{
    _metadata.color.colore[3] = alfa;
}

inline void Stroke::at_translation(const double zoom, point_s &point, const int indexPoint, const QPointF &translation) const
{
    memcpy(&point, &at(indexPoint), sizeof(point_s));

    point *= zoom;
    point += translation;
}

inline void Stroke::setColor(const QColor &color)
{
    _metadata.color.fromColor(color);
}

inline void Stroke::setColor(const colore_s &color)
{
    memcpy(&_metadata.color, &color, sizeof(_metadata.color));
}

inline void Stroke::copy(const Stroke &src, Stroke &dest)
{
    dest.reset();

    dest._point = src._point;
    dest._pressure = src._pressure;

    dest.setProp(src);

    dest._complex = stroke_complex_allocate(src, src._complex);

    dest._biggerData = src._biggerData;

    dest._flag = src._flag;

    memcpy(&dest._metadata, &src._metadata, sizeof(src._metadata));
}

inline Stroke &Stroke::operator=(const Stroke &other)
{
    if(unlikely(this == &other)){
        return *this;
    }

    Stroke::copy(other, *this);
    return *this;
}

inline bool Stroke::isEmpty() const
{
    return _point.isEmpty() && this->is_normal();
}

inline void Stroke::scale(const QPointF &offset)
{
    if(unlikely(is_complex())){
        return stroke_complex_translate(this, offset);
    }

    for(auto &point : _point){
        point += offset;
    }
}

/*
 * return -1 if it's not contained
 * if the stroke is complex and needToDeletePoint is
 * set, it is going to normalize the stroke
 *
 * if the rubber is set to remove the entire stroke
 * we don't have to normalize the stroke
*/
force_inline int Stroke::is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const
{
    int len;
    const point_s *p1, *p2;
    WLine tmp;

    int &i = from;

    W_ASSERT(from >= 0);

    if(unlikely(this->is_complex())){
        const int value = stroke_complex_is_inside(this, rect, precision);

        if(value){
            if(needToDeletePoint){
                Stroke tmp = *this;
                stroke_complex_make_normal(&tmp, (Stroke *)this);
                W_ASSERT(this->is_normal());
                goto continue_search;
            }else{
                return value;
            }
        }else{
            // no point found
            return -1;
        }
    }

continue_search:
    len = this->length();

    if(unlikely(!len))
        return -1;

    if(unlikely(i >= len))
        return -1;

    if(i == 0){
        const auto &ref = this->getBiggerPointInStroke();
        if(likely(!ref.intersects(rect.toRect().toRect())))
            return -1;
    }


    p1 = &at(i);
    for(i++; i < len; i++){
        p2 = &at(i);

        tmp = WLine(p1->toQPointF(1.), p2->toQPointF(1.));

        if(unlikely(WLine::intersect(tmp, rect, precision))){
            return i;
        }

        p1 = p2;
    }

    return -1;
}
