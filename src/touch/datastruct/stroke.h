#ifndef STROKE_H
#define STROKE_H

#include <QList>
#include <QImage>
#include <QPainterPath>
#include "point.h"
#include "zip.h"


#define VER_STROKE 0

struct metadata_stroke{
    int page;
    int idtratto;
    int posizione_audio;
    struct colore_s color;
};

class stroke
{
private:
    QList<point_s> m_point;

    metadata_stroke metadata;
    bool metadataSet: 1;
    int versione = VER_STROKE;

    QPainterPath path;

    QRectF biggerData;
    bool biggerDataSet: 1;

public:
    stroke();

    /* this function is used to set the pressure equal for all points. */
    void __setPressureForAllPoint(const double pressure);

    bool isIdUser() const;

    int save(zip_source_t *file);
    int load(zip_file_t *file);

    QColor getColor() const;

    const point_s   &at(const int index) const;
    point_s         &at_mod(const int index);

    void                append(const point_s &point) const;
    void                setMetadata(const metadata_stroke & metadata);
    void                setMetadata(const int page, const int idtratto,
                                    const int posizione_audio, const struct colore_s color);

    size_t createControll() const;

    int getId() const;
    int getPage() const;
    int getPosizioneAudio() const;

    QRectF getBiggerPointInStroke();
    bool isInside(const QRectF &rect) const;

    void clearAudio();

    int length() const;
    void changeId(const int newId);
    void commitChange();

    size_t getSize() const;
    void decreasePrecision() const;

    /* this function physically adds the x and y value of the point to all of its points. */
    void movePoint(const QPointF &translation);

    void getQPainterPath() const;
    void createQPainterPath();

    void reset();

    static void copy(const stroke &src, stroke &dest);
};

inline bool stroke::isIdUser() const
{
    return metadata.idtratto >= 0;
}

inline const point_s &stroke::at(const int index) const
{
    return this->m_point.at(index);
}

inline point_s &stroke::at_mod(const int index)
{
    return this->m_point.operator[](index);
}

inline void stroke::setMetadata(const metadata_stroke &metadata)
{
    Q_ASSERT(this->metadataSet == 0);
    memcpy(&this->metadata, &metadata, sizeof(this->metadata));
    metadataSet = 1;
}

// return the "old" idtratto
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
    if(this->biggerDataSet)
        return this->biggerData;

    int i, len = this->length();
    QRectF conf;
    conf.setTopLeft(QPointF(this->at(0).m_x, this->at(0).m_y));
    conf.setBottomRight(conf.topLeft());

    for (i = 0; i < len; i++){
        const point_s &point = at(i);
        if(point.m_x >= conf.x())
            conf.setX(point.m_x);
        if(point.m_y >= conf.y())
            conf.setY(point.m_y);
    }

    biggerDataSet = true;
    biggerData = conf;
    return biggerData;
}

inline void stroke::clearAudio()
{
    this->metadata.posizione_audio = -1;
}

inline int stroke::length() const
{
    return this->m_point.length();
}

inline void stroke::changeId(const int newId)
{
    this->metadata.idtratto = newId;
}

#endif // STROKE_H
