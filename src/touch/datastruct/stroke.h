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

struct metadata_stroke{
    int posizione_audio;
    struct colore_s color;
};

class stroke
{
private:
    QList<point_s> _point;

    struct metadata_stroke _metadata;

    QPainterPath _path;

    QRect _biggerData;
    bool _constantPressureVal;

    bool _needToCreatePanterPath;
    bool _needToCreateBiggerData;

    bool _needToUpdatePressure;

    void updateFlagPressure() const;

    void modify();

    int save(zip_source_t *file) const;
    int load(zip_file_t *file, int version);

public:
    stroke();
    stroke(const stroke &data);

    /* this function is used to set the pressure equal for all points. */
    void __setPressureForAllPoint(cdouble pressure);
    void __setPressureFirstPoint(cdouble pressuer);

    float getPressure() const;
    QColor getColor(const double division) const;

    const point_s   &at(const int index) const;
    point_s         &at_mod(const int index);

    void        append(const point_s &point);

    void    setMetadata(const int posizione_audio, const colore_s &color);
    void    setMetadata(const metadata_stroke &metadata);

    void setPositioneAudio(const int m_pos_ris);

    size_t createControll() const;

    int removeAt(int index);

    /* if you pass from = 0, to = 2 
     * will eliminate points with index 0, 1, 2
    */
    int removeAt(int from, int to); 

    int getPosizioneAudio() const;

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

    /* return true if equals */
    static bool cmp(const stroke &stroke1, const stroke &stroke2);
    static void copy(const stroke &src, stroke &dest);
    stroke &operator=(const stroke &other);

    bool isEmpty() const;

    const point_s &last() const;
    
#define STROKE_MUST_TRASLATE_PATH BIT(1)
    void scale(const QPointF &offset, int flag);

    friend class page;
};

inline void stroke::updateFlagPressure() const
{
    bool &__constPressureVal =      (bool &) _constantPressureVal;
    bool &__needToUpdatePressure =  (bool &) _needToUpdatePressure;

    int i, len;
    const point_s *current, *next;

    __needToUpdatePressure = false;
    len = this->length();

    if(unlikely(len < 3)){
        /* if we have less than 3 points we
         * cannot create a qpainterpath, so
         * we have to draw the stroke point
         * by point.
        */
        __constPressureVal = false;
        return;
    }

    current = &at(0);

    for (i = 0; i < len-1; i++){
        next = &at(i+1);

        if(next->pressure != current->pressure){
            __constPressureVal = false;
            return;
        }

        current = next;
    }

    __constPressureVal = true;
}

/* call this function when modify the stroke */
inline void stroke::modify()
{
    _needToUpdatePressure = true;
    _needToCreateBiggerData = true;
    _needToCreatePanterPath = true;
    _path = QPainterPath();
}

/* call this function only when constantPressureVal is 1 */
inline float stroke::getPressure() const
{
    return at(0).pressure;
}

inline QColor stroke::getColor(const double division = 1.0) const
{
    QColor color(_metadata.color.toQColor(division));
    return color;
}

inline const point_s &stroke::at(const int index) const
{
    return _point.at(index);
}

inline point_s &stroke::at_mod(const int index)
{
    this->modify();
    return _point.operator[](index);
}

inline void stroke::append(const point_s &point)
{
    this->_point.append(point);

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
    _point.removeAt(index);
    this->modify();

    return len < 2;
}

inline int stroke::removeAt(int from, int to){
    W_ASSERT(to < length());
    W_ASSERT(from >= 0);

    qDebug() << "stroke::removeAt from to" << from << to;

    for(; from <= to; to --){
        removeAt(from);
    }
    
    return 0;
}

inline int stroke::getPosizioneAudio() const
{
    return _metadata.posizione_audio;
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
    bool &__needToCreateBiggerData = (bool &) _needToCreateBiggerData;
    QRect &__biggerData = (QRect &) _biggerData;
    int count;

    if(likely(!_needToCreateBiggerData)){
        return _biggerData;
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

        if(topLeft.x() > point.m_x)
            topLeft.setX(point.m_x);

        if(topLeft.y() > point.m_y)
            topLeft.setY(point.m_y);

        if(bottomRight.x() < point.m_x)
            bottomRight.setX(point.m_x);

        if(bottomRight.y() < point.m_y)
            bottomRight.setY(point.m_y);
    }

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());

    __needToCreateBiggerData = false;

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
        if(point.m_x >= topLeft.x() && point.m_y >= topLeft.y() &&
                point.m_x <= bottomRight.x() && point.m_y <= bottomRight.y())
            return true;
    }

    return false;
}

inline void stroke::clearAudio()
{
    _metadata.posizione_audio = -1;
}

inline int stroke::length() const
{
    return _point.length();
}

inline const metadata_stroke &stroke::getMetadata() const
{
    return _metadata;
}

force_inline bool stroke::constantPressure() const
{
    if(unlikely(_needToUpdatePressure))
        this->updateFlagPressure();

    return _constantPressureVal;
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

    point.m_x *= zoom;
    point.m_y *= zoom;

    point.m_x += translation.x();
    point.m_y += translation.y();
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
    bool &__needToCreatePanterPath = (bool &)_needToCreatePanterPath;

    if(unlikely(_needToCreatePanterPath)){
        this->createQPainterPath(page);
        __needToCreatePanterPath = false;
    }

    return _path;
}

inline void stroke::copy(const stroke &src, stroke &dest)
{
    dest._point = src._point;
    dest._biggerData = src._biggerData;

    dest._needToCreateBiggerData = src._needToCreateBiggerData;
    dest._needToCreatePanterPath = src._needToCreatePanterPath;

    dest._needToUpdatePressure = src._needToUpdatePressure;

    dest._constantPressureVal = src._constantPressureVal;
    memcpy(&dest._metadata, &src._metadata, sizeof(src._metadata));

    dest._needToCreatePanterPath = src._needToCreatePanterPath;

    dest._path = src._path;
}

inline stroke &stroke::operator=(const stroke &other)
{
    if(this == &other){
        return *this;
    }

    stroke::copy(other, *this);
    return *this;
}

inline bool stroke::isEmpty() const
{
    return _point.isEmpty();
}

inline void stroke::scale(const QPointF &offset, int flag)
{
    int i = this->length();
    for(i--; i >= 0; i--){
        point_s &point = at_mod(i);

        point.m_x += offset.x();
        point.m_y += offset.y();
    }

    if((flag & STROKE_MUST_TRASLATE_PATH) && likely(!_needToCreatePanterPath) && this->constantPressure())
        _path.translate(offset);
    else
        _path = QPainterPath();
}


#endif // STROKE_H
