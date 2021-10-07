#include "datastruct.h"
#include <QList>
#include <QDebug>

int datastruct::minId()
{
    if(isempty())
        return 0;

    /*
     * we initialize the id to zero
     * as the traits have a negative id
    */
    int _minId = 0;
    uint i, len;
    const point_s * __point = firstPoint();

    len = length();
    _minId = __point->idtratto;

    for(i=0; i<len; i++){
        __point = at(i);
        if(__point->idtratto < _minId){
            _minId = __point->idtratto;
        }
    }

    return _minId;
}

#define mov_if_neg(p, x) \
    p = x; \
    return;

void datastruct::moveIfNegative(uint &p, const uint len,
                                const uint height,
                                const uint width){
    uint r;
    const point_s *f, *s;

    r = p;

    for(; p<len-1; ++p){

        f = at(p);
        s = at(p+1);

        if(!datastruct::isIdUser(f))
            return;

        if((f->m_y >= 0.0 && f->m_x >= 0.0) &&
                (f->m_y <= height && f->m_x <= width)){
            mov_if_neg(p, r);
            return;
        }

        if(s->idtratto == f->idtratto
                && std::abs((f->m_y + s->m_y)/2) < height
                && std::abs((f->m_x + s->m_x)/2) < width){
            mov_if_neg(p, r);
        }

        if(f->idtratto != s->idtratto){
            r = p;
        }
    }
}

uint datastruct::move_to_positive(uint len)
{
    uint i;
    if(!len)
        len = length();

    for(i=0; i<len; ++i){
        if(at(i)->m_y >= 0.0)
            return i;
    }

    return 0;
}

/*
 * return the index of the first point with
 * idtratto == IDORIZZONALE
*/
unsigned int datastruct::posIdOrizzonal()
{
    uint i, len;
    len = m_point.length();

    for(i=0; i<len; ++i){
        if(at(i)->idtratto == IDORIZZONALE)
            return i;
    }

    return 0;
}

datastruct::datastruct(frompdf *m_pdf, fromimage *m_img)
{
    this->m_pdf = m_pdf;
    this->m_img = m_img;
    __last_translation.setX(0);
    __last_translation.setY(0);
}

/*
 * the function take a pointer to a unsigned
 * int and move the index to the next point
 * with a differente id
*/
void datastruct::moveNextPoint(uint *pos,
                               uint len,
                               int id)
{
    if(id == -6)
        id = at(*pos)->idtratto;

    if(!len)
        len = length();

    for(;at(*pos)->idtratto == id && *pos < len; ++(*pos));
}

bool datastruct::maxXIdOrizzonal(double *val)
{
    int i, len = m_point.length();


    /*
     * in this point of the code we are assuming
     * that the section is composed of two points,
     * in case of data loss it does not warn
    */
    for(i=0; i<len; i++){
        if(m_point.at(i).idtratto == IDORIZZONALE)
            goto return_;
    }

    return false;

    return_:
    *val = m_point.at(i+1).m_x;
    return true;
}

bool datastruct::minXIdOrizzonal(double *val)
{
    if(this->isempty())
        return false;

    unsigned int i = this->posIdOrizzonal();

    if(!i)
        return false;

    *val = m_point.at(i).m_x;

    return true;
}

/* the function returns true if the id is available */
bool datastruct::isAvailable(int id)
{
    int i, len = m_point.length();
    for(i=0; i<len; ++i)
        if(m_point.at(i).idtratto == id)
            return 0;


    return 1;
}

void datastruct::reset(){
    m_point.clear();

    zoom = 1.00;
    posizionefoglio.clear();
}

uint datastruct::positionId(int id)
{
    unsigned int len, i;
    len = m_point.length();

    for(i=0; i<len; i++){
        if(m_point.at(i).idtratto == id)
            return i;
    }

    return 0;
}

uint datastruct::decreaseAlfa(int id,
                                  uchar decrease,
                                  uint len)
{
    uint i;
    point_s *__point;

    for(i=0; i<len; ++i){
        __point = at_mod(i);

        if(__point->idtratto == id){
            __point->m_color.colore[3] /= decrease;
        }
    }

    return i;
}

double datastruct::miny()
{
    unsigned int i, len;

    len = m_point.length();

    const point_s * __point = &m_point.first();

    double min = __point->m_y;

    for(i=0; i<len; ++i){
        __point = &m_point.at(i);

        if(__point->m_y < min){
            min = __point->m_y;
        }

    }

    return min;

}

double datastruct::minx()
{
    unsigned int i, len;

    len = m_point.length();

    const point_s * __point = &m_point.first();

    double min = __point->m_x;

    for(i=0; i<len; ++i){
        __point = &m_point.at(i);

        if(__point->m_x < min){
            min = __point->m_x;
        }

    }

    return min;
}


double datastruct::biggerynoid() const{
    if(m_point.isEmpty())
        return (double)0;

    uint i, len;
    double y_ = m_point.first().m_y;

    const point_s * __point;

    len = length();

    for(i=0; i<len; ++i){
        __point = at(i);

        if(__point->m_y > y_
                && datastruct::isIdUser(__point)){
                y_ = m_point.at(i).m_y;
        }
    }

    return y_;

}

void datastruct::removeat(int i){
    m_point.removeAt(i);

}

/*No IDVERTICALE ORIZZONTALE*/
double datastruct::biggerxNoId(){
    int i, len;
    double max;

    len = m_point.length();
    if(!len)
        return 0;

    const point_s * __point = &m_point.first();


    max = __point->m_x;


    for(i=0; i<len; i++){
        __point = &m_point.at(i);

        if(max < __point->m_x
                && isIdUser(__point)){
            max = __point->m_x;
        }
    }

    return max;

}


/*
 * the function return true if the point in position i
 * is in the middle of a stretch
 *
 * return false if it's the first point or the lastone
 */
bool datastruct::needtochangeid(const unsigned int i){
    unsigned int len;
    len = m_point.length();

    if(!i || (i+1) >= len)
        return false;

    int temp_id = m_point.at(i).idtratto;

    return (temp_id == m_point.at(i-1).idtratto
            && temp_id == m_point.at(i+1).idtratto);

}
