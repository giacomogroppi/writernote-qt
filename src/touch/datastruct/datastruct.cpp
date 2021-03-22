#include "datastruct.h"
#include <QList>
#include "../method/methoddefinition.h"

datastruct::datastruct()
{

}

bool datastruct::repositioning()
{
    if(m_point.isEmpty() || m_point.first().m_x == 0.0 || m_point.first().m_y == 0)
        return false;

    QPointF point_temp(-m_point.first().m_x, -m_point.first().m_y);
    this->scala_all(point_temp);

    return true;
}

QPointF datastruct::scala_all()
{
    if(this->m_point.isEmpty())
        return QPointF(0, 0);

    QPointF point(-m_point.first().m_x, -m_point.first().m_y);

    scala_all(point);

    return point;
}

void datastruct::scala_all(QPointF &point)
{
    this->scala_x(point.x());
    this->scala_y(point.y());
}

void datastruct::scala_x(double scala)
{
    int i, len;
    len = this->m_point.length();
    for(i=0; i<len; i++){
        m_point.operator[](i).m_x += scala;
    }
}

void datastruct::scala_y(double scala)
{
    int i, len;
    len = this->m_point.length();
    for(i=0; i<len; i++){
        m_point.operator[](i).m_y += scala;
    }
}

void datastruct::reset(){
    m_point.clear();

    /*
     * last data struct
    */
    /*
    this->color.clear();
    this->x.clear();
    this->y.clear();
    this->posizioneaudio.clear();
    this->idtratto.clear();
    this->zoom = 1.00;
    this->posizionefoglio.clear();
    */
}

double datastruct::biggery(){
    if(m_point.isEmpty())
        return (double)0;

    int i, len;
    double y_ = m_point.first().m_y;
    len = m_point.length();

    for(i=0; i<len; i++){
        if(m_point.at(i).m_y > y_)
            y_ = m_point.at(i).m_y;
    }
    return y_;

}

unsigned int datastruct::positionId(int id)
{
    unsigned int len, i;
    len = m_point.length();

    for(i=0; i<len; i++){
        if(m_point.at(i).idtratto == id)
            return i;
    }

    return 0;
}

static inline void d(point_s *point, int alfa){
    point->m_color.colore[3] /= alfa;
}

void datastruct::decreaseAlfa(int id, int decrease)
{
    int i, len = this->m_point.length();

    for(i=0; i<len; i++)
        if(m_point.at(i).idtratto == id)
            d(&m_point.operator[](i), decrease);
}


auto datastruct::biggerynoid() -> double{
    if(m_point.isEmpty())
        return (double)0;

    int i, len, temp;
    double y_ = m_point.first().m_y;
    len = m_point.length();

    for(i=0; i<len; i++){
        if(m_point.at(i).m_y > y_){
            temp = m_point.at(i).idtratto;
            if(temp != IDVERTICALE && temp != IDORIZZONALE){
                y_ = m_point.at(i).m_y;
            }
        }
    }
    return y_;

}

void datastruct::removeat(int i){
    m_point.removeAt(i);

}

/*YES IDVERTICALE ORIZZONTALE*/
double datastruct::biggerx(){
    int i, len;
    double max;

    len = m_point.length();
    max = m_point.first().m_x;

    for(i=0; i<len; i++){
        if(max < m_point.at(i).m_x){
            max = m_point.at(i).m_x;
        }
    }

    /*
     * last data struct
    */
    /*
    len = x.length();

    if(!len)
        return 0;
    max = x.first();
    for(i=0; i < len; i++)
        if(max < x.at(i))
            max = x.at(i);*/

    return max;

}

/*No IDVERTICALE ORIZZONTALE*/
double datastruct::biggerxNoId(){
    int i, len;
    double max;

    len = m_point.length();
    max = m_point.first().m_x;

    for(i=0; i<len; i++){
        if(max < m_point.at(i).m_x && m_point.at(i).idtratto != IDVERTICALE && m_point.at(i).idtratto != IDORIZZONALE){
            max = m_point.at(i).m_x;
        }
    }

    return max;

}


/*
 * the function return true if the point to delete is in the
 * middle of a stretch
 *
 * return false if it's the first point or the lastone
 */
bool datastruct::needtochangeid(int i){
    int len;
    len = m_point.length();

    if(!i || i == len)
        return false;

    int temp_id = m_point.at(i).idtratto;

    if(temp_id == m_point.at(i-1).idtratto
            && temp_id == m_point.at(i+1).idtratto)
        return true;

    /*
     * last data struct
    */
    /*
    len = y.length();

    if(i == len || i == 0)
        return false;

    if(idtratto.at(i) == idtratto.at(i+1) && idtratto.at(i-1) == idtratto.at(i))
        return true;
    */
    return false;

}

/* max y in the list */
double datastruct::last(){
    if(m_point.isEmpty())
        return (double)0;

    double max = m_point.first().m_y;
    int i, len;
    len = m_point.length();

    for(i=0; i<len; i++){

    }

    /*
     * last data struct
    */
    /*
    if(y.isEmpty())
        return 0;

    int i, len = this->y.length(), max = this->y.at(0);
    for(i = 0; i < len; i++){
        if(max < this->y.at(i) && idtratto.at(i) != -1)
            max = this->y.at(i);
    }*/

    return max;
}
