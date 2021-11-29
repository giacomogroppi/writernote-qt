#ifndef STROKE_H
#define STROKE_H

#include <QList>
#include <QImage>
#include <QPainterPath>
#include "point.h"
#include "zip.h"
#include "../../utils/color/setcolor.h"
#include "../../utils/common_def.h"

#define VER_STROKE 0

struct metadata_stroke{
    int page;
    int idtratto = -1;
    int posizione_audio;
    struct colore_s color;
};

class stroke
{
private:
    QList<point_s> m_point;

    struct metadata_stroke metadata;
    int versione = VER_STROKE;

    QPainterPath path;

    QRectF biggerData;
    bool constantPressureVal: 1;

    bool needToCreatePanterPath: 1;
    bool needToCreateBiggerData: 1;

    void updateFlagPressure();

    void modify();

public:
    stroke();
    stroke(const stroke &data);

    /* this function is used to set the pressure equal for all points. */
    void __setPressureForAllPoint(const double pressure);

    int save(zip_source_t *file) const;
    int load(zip_file_t *file);

    float getPressure() const;
    QColor getColor(const double division) const;

    const point_s   &at(const int index) const;
    point_s         &at_mod(const int index);

    void                append(const point_s &point);
    void                setMetadata(const int page, const int idtratto, const int posizione_audio, const struct colore_s color);

    void setPage(int page);
    void setPositioneAudio(const int m_pos_ris);
    void setId(const int id);
    size_t createControll() const;

    void removeAt(int index);

    int getId() const;
    int getPage() const;
    int getPosizioneAudio() const;

    QRectF getBiggerPointInStroke();
    bool isInside(const QRectF &rect) const;

    void clearAudio();

    int length() const;

    bool constantPressure() const;

    size_t getSizeInMemory() const;
    void decreasePrecision();

    void setAlfaColor(const uchar alfa);

    __slow void at_translation(const double zoom, point_s &point, const int indexPoint, const QPointF &translation) const;

    void setColor(const QColor &color);
    void setColor(const colore_s &color);

    /* this function physically adds the x and y value of the point to all of its points. */
    void movePoint(const QPointF &translation);

    QPainterPath getQPainterPath();
    void createQPainterPath();

    void reset();

    /* return true if equals */
    static bool cmp(const stroke &stroke1, const stroke &stroke2);
    static void copy(const stroke &src, stroke &dest);
    stroke &operator=(const stroke &other);

    /* debug */
    QString toString() const;
};

inline void stroke::updateFlagPressure()
{
    int i, len;
    const point_s *point, *point2;

    len = this->length();

    point = &at(0);
    for (i = 0; i < len-1; i++){
        point2 = &at(i+1);
        if(at(i).pressure != point->pressure){
            this->constantPressureVal = false;
            return;
        }
        point = point2;
    }

    this->constantPressureVal = true;
}

/* call this function when modify the stroke */
inline void stroke::modify()
{
    this->needToCreateBiggerData = true;
    this->needToCreatePanterPath = true;
}

/* call this function only when constantPressureVal is 1 */
inline float stroke::getPressure() const
{
    return at(0).pressure;
}

inline QColor stroke::getColor(const double division = 1.0) const
{
    return setcolor(this->metadata.color, division);
}

inline const point_s &stroke::at(const int index) const
{
    return this->m_point.at(index);
}

inline point_s &stroke::at_mod(const int index)
{
    return this->m_point.operator[](index);
}

inline void stroke::append(const point_s &point)
{
    this->m_point.append(point);
    this->updateFlagPressure();
    this->modify();
}

inline void stroke::setPage(int page)
{
    this->metadata.page = page;
}

inline void stroke::setPositioneAudio(const int m_pos_ris)
{
    this->metadata.posizione_audio = m_pos_ris;
}

inline void stroke::setId(const int id)
{
    this->metadata.idtratto = id;
}

inline void stroke::removeAt(int index)
{
    this->m_point.removeAt(index);
    this->modify();
}

inline int stroke::getId() const
{
    return metadata.idtratto;
}

inline int stroke::getPage() const
{
    return this->metadata.page;
}

inline int stroke::getPosizioneAudio() const
{
    return this->metadata.posizione_audio;
}

/* after append data we need to call this funcion to update */
inline QRectF stroke::getBiggerPointInStroke()
{
    if(!this->needToCreateBiggerData){
        return this->biggerData;
    }

    //qDebug() << "stroke::getBiggerPointInStroke call";

    int i, len = this->length();
    const point_s &first = at(0);
    QPointF topLeft(first.m_x, first.m_y);
    QPointF bottomRight(topLeft);

    for (i = 0; i < len; i++){
        const point_s &point = at(i);

        if(topLeft.x() > point.m_x)
            topLeft.setX(point.m_x);
        if(bottomRight.x() < point.m_x)
            bottomRight.setX(point.m_y);


        if(topLeft.y() > point.m_y)
            topLeft.setY(point.m_y);
        if(bottomRight.y() < point.m_y)
            bottomRight.setY(point.m_y);
    }

    if(bottomRight.x() < topLeft.x()){
        const double x = topLeft.x();
        topLeft.setX(bottomRight.x());
        bottomRight.setX(x);
    }

    if(topLeft.x() > bottomRight.y()){
        const double y = topLeft.y();
        topLeft.setY(bottomRight.y());
        bottomRight.setY(y);
    }

    this->needToCreateBiggerData = false;

    biggerData = QRectF(topLeft, bottomRight);
    return biggerData;
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
        if(point.m_x >= topLeft.x() && point.m_y >= topLeft.y() &&
                point.m_x <= bottomRight.x() && point.m_y <= bottomRight.y())
            return true;
    }

    return false;
}

inline void stroke::clearAudio()
{
    this->metadata.posizione_audio = -1;
}

inline int stroke::length() const
{
    return this->m_point.length();
}

inline bool stroke::constantPressure() const
{
    return this->constantPressureVal;
}

inline void stroke::setAlfaColor(const uchar alfa)
{
    this->metadata.color.colore[3] = alfa;
    this->needToCreatePanterPath = true;
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
    setcolor_struct(metadata.color, color);
}

inline void stroke::setColor(const colore_s &color)
{
    memcpy(&this->metadata.color, &color, sizeof(metadata.color));
}

inline QPainterPath stroke::getQPainterPath()
{
    if(this->needToCreatePanterPath)
        this->createQPainterPath();
    return this->path;
}

inline void stroke::copy(const stroke &src, stroke &dest)
{
    dest.m_point = src.m_point;
    dest.biggerData = src.biggerData;

    dest.needToCreateBiggerData = src.needToCreateBiggerData;
    dest.needToCreatePanterPath = src.needToCreatePanterPath;

    dest.constantPressureVal = src.constantPressureVal;
    memcpy(&dest.metadata, &src.metadata, sizeof(src.metadata));

    dest.needToCreatePanterPath = src.needToCreatePanterPath;

    dest.path = src.path;

    dest.versione = src.versione;
}

inline stroke &stroke::operator=(const stroke &other)
{
    if(this == &other){
        return *this;
    }

    stroke::copy(other, *this);
    return *this;
}

inline QString stroke::toString() const
{
    QString message;
    message += QString::number((size_t) this) + " length" + QString::number(length());
    message += " id" + QString::number(this->metadata.idtratto);

    return message;
}


#endif // STROKE_H
