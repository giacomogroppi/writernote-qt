#ifndef STROKE_H
#define STROKE_H

#include <QList>
#include <QImage>
#include <QDebug>
#include <QRect>
#include <QPainterPath>
#include "point.h"
#include "zip.h"
#include "utils/common_def.h"
#include "utils/common_script.h"
#include "touch/datastruct/utils_datastruct.h"
#include "core/wline.h"
#include "touch/datastruct/stroke_complex_data.h"
#include "touch/object_finder/model/model.h"

struct metadata_stroke{
    int posizione_audio;
    struct colore_s color;
};

typedef float pressure_t;

class stroke
{
private:
    QList<point_s> _point;
    QList<pressure_t> _pressure;

    struct metadata_stroke _metadata;

    QPainterPath _path;

    QRect _biggerData;

    enum flag_status : unsigned char{
        UPDATE_PANTER_PATH = BIT(1),
        UPDATE_BIGGER_DATA = BIT(2),
        UPDATE_PRESSURE = BIT(3)
    };

    unsigned char _flag;

    bool isPressureVal() const;
    bool needToCreatePanterPath() const;
    bool needToUpdatePressure() const;
    bool needToUpdateBiggerData() const;

    int _prop = COMPLEX_NORMAL;
    static_assert(sizeof(_prop) == 4);

    void *_complex = NULL;
    void setFlag(unsigned char type, bool value) const;

    static_assert(sizeof(_flag) * 8 >= 4 );

    void updateFlagPressure() const;

    void modify();

    int save(zip_source_t *file) const;
    int load(zip_file_t *file, int version);

public:
    stroke();
    stroke(const stroke &data);
    ~stroke();

    enum flag_complex : typeof(_prop){
        COMPLEX_NORMAL = 0,
        COMPLEX_CIRCLE = 1,
        COMPLEX_RECT = 2,
        COMPLEX_LINE = 3
    };

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
    int is_inside(const WLine &rect, int from, int precision) const;

    void __setPressureFirstPoint(const pressure_t pressure);

    pressure_t getPressure(int index) const;
    pressure_t getPressure() const;
    QColor getColor(const double division) const;

    const point_s   &at(const int index) const;
    point_s         &at_mod(const int index);

#define stroke_append_default -1.
    void append(const point_s &point, pressure_t pressure);

    void setMetadata(const int posizione_audio, const colore_s &color);
    void setMetadata(const metadata_stroke &metadata);

    void setPositioneAudio(const int m_pos_ris);

    size_t createControll() const;

    int removeAt(int index);

    /* if you pass from = 0, to = 2 
     * will eliminate points with index 0, 1, 2
    */
    int removeAt(int from, int to); 

    int getPosizioneAudio() const;
    QRect getFirstAndLast() const;
    QRect getBiggerPointInStroke() const;
    bool isInside(const QRectF &rect) const;

    void clearAudio();

    int length() const;
    const struct metadata_stroke &getMetadata() const;
    bool constantPressure() const;

    uchar get_alfa() const;

    size_t getSizeInMemory() const;
    void decreasePrecision();

    void setAlfaColor(const uchar alfa);

    __slow void at_translation(const double zoom, point_s &point, const int indexPoint, const QPointF &translation) const;

    void setColor(const QColor &color);
    void setColor(const colore_s &color);

    /* this function physically adds the x and y value of the point to all of its points. */
    void movePoint(const QPointF &translation);

    const QPainterPath &getQPainterPath(int page) const;

    void createQPainterPath(int page) const;

    void reset();

    stroke &operator=(const stroke &other);

    bool isEmpty() const;

    const point_s &last() const;
    
#define STROKE_MUST_TRASLATE_PATH BIT(1)
    void scale(const QPointF &offset, int flag);

    force_inline bool is_normal() const { return _prop == COMPLEX_NORMAL; };
    force_inline bool is_circle() const { return _prop == COMPLEX_CIRCLE; };
    force_inline bool is_rect() const { return _prop == COMPLEX_RECT; };
    force_inline bool is_line() const { return _prop == COMPLEX_LINE; };
    void set_complex(typeof(_prop) new_prop, void *new_data);
    const void *get_complex_data() const { return _complex; };
    typeof(_prop) get_type() const { return _prop; };

    static bool cmp(const stroke &stroke1, const stroke &stroke2);
    static void copy(const stroke &src, stroke &dest);

    friend class page;
    friend class xmlstruct;
    friend class stroke_drawer;
    friend void stroke_complex_adjust(stroke *stroke, cdouble zoom);
};

force_inline void stroke::set_complex(typeof(_prop) new_prop, void *new_data)
{
    if(_complex){
        free(_complex);
    }

    _prop = new_prop;
    _complex = new_data;
}

force_inline bool stroke::isPressureVal() const
{
    return _pressure.length() == 1;
}

force_inline bool stroke::needToCreatePanterPath() const
{
    return _flag & UPDATE_PANTER_PATH;
}

force_inline bool stroke::needToUpdatePressure() const
{
    return _flag & UPDATE_PRESSURE;
}

force_inline bool stroke::needToUpdateBiggerData() const
{
    return _flag & UPDATE_BIGGER_DATA;
}

force_inline void stroke::setFlag(unsigned char type, bool value) const
{
    uchar &__flag = (uchar &) _flag;
    if(value){
        __flag |= type;
    }else{
        __flag &= ~type;
    }
}

inline void stroke::updateFlagPressure() const
{
    constexpr const auto typeUpdate = UPDATE_PRESSURE;
    int len, i;
    pressure_t press;
    auto &_press = (QList<pressure_t> &)_pressure;

    setFlag(typeUpdate, false);

    len = this->length();

    if(unlikely(len != 1 && _press.length() == 1)){
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
force_inline void stroke::modify()
{
    _flag = UPDATE_BIGGER_DATA | UPDATE_PRESSURE | UPDATE_PANTER_PATH;

    W_ASSERT(this->needToCreatePanterPath());
    W_ASSERT(this->needToUpdateBiggerData());
    W_ASSERT(this->needToUpdatePressure());

    _path = QPainterPath();
}

force_inline stroke::~stroke()
{
    this->reset();
}

inline pressure_t stroke::getPressure(int index) const
{
    int __index = 0;

    if(_pressure.length() > 1){
        __index = index;
    }

    return _pressure.at(__index);
}

/* call this function only when constantPressureVal is 1 */
inline pressure_t stroke::getPressure() const
{
    W_ASSERT(this->is_normal());
    return _pressure.at(0);
}

inline QColor stroke::getColor(const double division = 1.) const
{
    return _metadata.color.toQColor(division);
}

inline const point_s &stroke::at(const int index) const
{
    W_ASSERT(_prop == COMPLEX_NORMAL);
    return _point.at(index);
}

inline point_s &stroke::at_mod(const int index)
{
    W_ASSERT(_prop == COMPLEX_NORMAL);
    this->modify();
    return _point.operator[](index);
}

inline void stroke::append(const point_s &point, pressure_t pressure)
{
    /*
     * Appende sempre la pressione del punto, la prima volta che viene
     * aggiornato il flag deciderà se cancellare o meno la list delle
     * pressioni, in caso siano tutte uguali
    */
    this->_point.append(point);
    _pressure.append(pressure);

    this->modify();
}

inline void stroke::setMetadata(const metadata_stroke &metadata)
{
    memcpy(&_metadata, &metadata, sizeof(_metadata));
}

inline void stroke::setPositioneAudio(const int m_pos_ris)
{
    _metadata.posizione_audio = m_pos_ris;
}

/* return 1 if the stroke need to be delete */
inline int stroke::removeAt(int index)
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

inline int stroke::removeAt(int from, int to){
    W_ASSERT(to < length());
    W_ASSERT(from >= 0);

    constexpr bool debug_remove_at = false;

    WDebug(debug_remove_at, "stroke::removeAt from to" << from << to);

    for(; from <= to; to --){
        removeAt(from);
    }
    
    return 0;
}

inline int stroke::getPosizioneAudio() const
{
    return _metadata.posizione_audio;
}

force_inline QRect stroke::getFirstAndLast() const
{
    const auto &first = at(0);
    const auto &last  = at(length() - 1);
    return QRect(first.toQPointF(1. ).toPoint(),
                   last.toQPointF(1.  ).toPoint());
}

/* after append data we need to call this funcion to update
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
inline QRect stroke::getBiggerPointInStroke() const
{
    QRect &__biggerData = (QRect &) _biggerData;
    int count;

    if(likely(!needToUpdateBiggerData())){
        return _biggerData;
    }

    if(!this->is_normal()){
        __biggerData = stroke_complex_bigger_data(this);
        return __biggerData;
    }

    count = this->length();

    if(unlikely(count == 0)){
        qDebug() << "Warning: Stroke empty";
        return QRect(0, 0, 0, 0);
    }

    count --;

    QPoint topLeft(at(0).toQPointF(1.0).toPoint());
    QPoint bottomRight(at(count).toQPointF(1.0).toPoint());

    for (; count >= 0; count --){
        const point_s &point = at(count);

        if(topLeft.x() > point._x)
            topLeft.setX(point._x);

        if(topLeft.y() > point._y)
            topLeft.setY(point._y);

        if(bottomRight.x() < point._x)
            bottomRight.setX(point._x);

        if(bottomRight.y() < point._y)
            bottomRight.setY(point._y);
    }

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());

    setFlag(UPDATE_BIGGER_DATA, false);

    __biggerData = QRect(topLeft, bottomRight);

    return __biggerData;
}

/* you have to pass a rectangle already
 * translated with respect to the translation
 * point in datastruct, and already
 * multiplied by the zoom. */
inline bool stroke::isInside(const QRectF &rect) const
{
    int i;
    const int len = this->length();
    const QPointF &topLeft = rect.topLeft();
    const QPointF &bottomRight = rect.bottomRight();

    for(i = 0; i < len; i++){
        const point_s &point = at(i);

        /* TODO --> use the function in datastruct */
        if(point._x >= topLeft.x() && point._y >= topLeft.y() &&
                point._x <= bottomRight.x() && point._y <= bottomRight.y())
            return true;
    }

    return false;
}

inline void stroke::clearAudio()
{
    _metadata.posizione_audio = -1;
}

force_inline int stroke::length() const
{
    W_ASSERT(this->is_normal());
    return _point.length();
}

inline const metadata_stroke &stroke::getMetadata() const
{
    return _metadata;
}

force_inline bool stroke::constantPressure() const
{
    if(unlikely(needToUpdatePressure()))
        this->updateFlagPressure();

    return isPressureVal();
}

force_inline uchar stroke::get_alfa() const
{
    return _metadata.color.colore[3];
}

inline void stroke::setAlfaColor(const uchar alfa)
{
    _metadata.color.colore[3] = alfa;
}

inline void stroke::at_translation(const double zoom, point_s &point, const int indexPoint, const QPointF &translation) const
{
    memcpy(&point, &at(indexPoint), sizeof(point_s));

    point._x *= zoom;
    point._y *= zoom;

    point._x += translation.x();
    point._y += translation.y();
}

inline void stroke::setColor(const QColor &color)
{
    _metadata.color.fromColor(color);
}

inline void stroke::setColor(const colore_s &color)
{
    memcpy(&_metadata.color, &color, sizeof(_metadata.color));
}

inline const QPainterPath &stroke::getQPainterPath(int page) const
{
    if(unlikely(needToCreatePanterPath())){
        this->createQPainterPath(page);
        setFlag(UPDATE_PANTER_PATH, false);
    }

    return _path;
}

inline void stroke::copy(const stroke &src, stroke &dest)
{
    dest.reset();

    dest._point = src._point;
    dest._pressure = src._pressure;
    dest._prop = src._prop;

    dest._complex = stroke_complex_allocate(src._prop, src._complex);

    dest._biggerData = src._biggerData;

    dest._flag = src._flag;

    memcpy(&dest._metadata, &src._metadata, sizeof(src._metadata));

    dest._path = src._path;
}

inline stroke &stroke::operator=(const stroke &other)
{
    if(unlikely(this == &other)){
        return *this;
    }

    stroke::copy(other, *this);
    return *this;
}

inline bool stroke::isEmpty() const
{
    return _point.isEmpty() && this->is_normal();
}

inline void stroke::scale(const QPointF &offset, int flag)
{
    int i = this->length();
    for(i--; i >= 0; i--){
        point_s &point = at_mod(i);

        point._x += offset.x();
        point._y += offset.y();
    }

    if((flag & STROKE_MUST_TRASLATE_PATH) && likely(!needToCreatePanterPath()) && this->constantPressure())
        _path.translate(offset);
    else
        _path = QPainterPath();
}

/*
 * return -1 if it's not contained
*/
force_inline int stroke::is_inside(const WLine &rect, int from, int precision) const
{
    int len;
    const point_s *p1, *p2;
    WLine tmp;

    int &i = from;

    W_ASSERT(from >= 0);

    len = this->length();

    if(unlikely(!len))
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

        if(unlikely(tmp.intersect(rect, precision))){
            return i;
        }

        p1 = p2;
    }

    return -1;
}

#endif // STROKE_H
