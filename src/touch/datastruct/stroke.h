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
    int metadataSet = 0;
    int versione = VER_STROKE;

    QPainterPath path;

public:
    stroke();

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

    int getId() const;
    int getPage() const;
    int getPosizioneAudio() const;


    int length() const;

    void commitChange();

    void getQPainterPath() const;
    void createQPainterPath();

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

inline int stroke::length() const
{
    return this->m_point.length();
}

#endif // STROKE_H
