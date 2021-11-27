#include "stroke.h"
#include "page.h"
#include "../../utils/color/setcolor.h"
#include <QDebug>

stroke::stroke()
{
    reset();
}

stroke::stroke(const stroke &data)
{
    reset();
    *this = data;
}

void stroke::__setPressureForAllPoint(const double pressure)
{
    uint i, lenPoint;
    lenPoint = length();

    for(i = 0; i < lenPoint; i++){
        this->at_mod(i).pressure = pressure;
    }
}

int stroke::save(zip_source_t *file) const
{

}

int stroke::load(zip_file_t *file)
{

}

void stroke::setMetadata(const int page, const int idtratto, const int posizione_audio, const colore_s color)
{
    this->metadata.page = page;
    this->metadata.idtratto = idtratto;
    this->metadata.posizione_audio = posizione_audio;
    memcpy(&this->metadata.color, &color, sizeof(color));
}

size_t stroke::createControll() const
{
    size_t controll = 0;
    int i;
    for(i = 0; i < NCOLOR; i++){
        controll += this->metadata.color.colore[i];
    }

    controll += this->metadata.idtratto;

    for(i = 0; i < length(); i++){
        const point_s &point = m_point.at(i);
        controll += point.m_x;
        controll += point.m_y;
        controll += point.pressure;
    }

    return controll;
}

size_t stroke::getSizeInMemory() const
{
    return sizeof(point_s) * length();
}

void stroke::decreasePrecision()
{
    int i, len;
    len = length();

    for(i = 1; i < len - 1; i++){
        if(i % 2 == 0){
            this->removeAt(i);
        }
    }
}

void stroke::movePoint(const QPointF &translation)
{
    uint i;
    const uint len = this->length();

    for(i = 0; i < len; i++){
        point_s &point = at_mod(i);
        point.m_x += translation.x();
        point.m_y += translation.y();
    }

}

void stroke::createQPainterPath()
{
    int i, len;
    const double delta = PROP_RESOLUTION;
    //const int decrease = (this->metadata.posizione_audio > m_pos_ris) ? 1 : 4;
    const point_s *point, *point1, *point2;

    len = this->length();

    for(i=0; i < len-3; ++i){
        path.moveTo(at(i).toQPointF(delta));

        while(i<len-3){
            point =     page::at_translation(at(i), this->metadata.page);
            point1 =    page::at_translation(at(i+1), this->metadata.page);
            point2 =    page::at_translation(at(i+2), this->metadata.page);

            path.cubicTo(point->toQPointF(delta), point1->toQPointF(delta), point2->toQPointF(delta));

            i += 2;
        }

    }
}

void stroke::reset()
{
    this->biggerDataSet = false;
    this->metadataSet = false;
    this->constantPressureVal = false;
    this->needToCreatePanterPath = false;

    this->m_point.clear();
    this->path = QPainterPath();
}

bool stroke::cmp(const stroke &stroke1, const stroke &stroke2)
{
    int i;
    const int len = stroke1.length();
    if(len != stroke2.length())
        return false;
    for (i = 0; i < len; i ++){
        const point_s &point1 = stroke1.at(i);
        const point_s &point2 = stroke2.at(i);
        if(memcmp(&point1, &point2, sizeof(point1)) != 0){
            return false;
        }
    }

    return true;
}
