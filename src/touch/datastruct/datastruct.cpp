#include "datastruct.h"
#include <QList>
#include "../method/methoddefinition.h"

datastruct::datastruct()
{

}

/* the function restores the ids, as
 * there may be holes, when we go
 * to eliminate some strokes written
 * with the pen
*/
void datastruct::reorganize()
{
    int i, len, last, k;
    len = m_point.length();

    if(!len)
        return;

    last = m_point.first().idtratto;

}

bool datastruct::maxXIdOrizzonal(double *val)
{
    int i, len = m_point.length();

    double pos = m_point.first().m_x;

    point_s & __point = m_point.first();

    for(i=0; i<len; i++){
        __point = m_point.at(i);
        if(__point.m_x > pos && __point.idtratto == IDORIZZONALE)
            goto return_;
    }

    *val = 0;
    return false;

    return_:
    *val = m_point.at(i).m_x;
    return true;
}

/*
 * this function change the id of all point
 * with the same id of m_point[i]
*/
void datastruct::changeId(unsigned int i)
{
    int id = maxId() + 1;
    int temp = m_point.at(i).idtratto;

    for(; m_point.at(i).idtratto == temp; i++)
        m_point.operator[](i).idtratto = id;
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

/*
 * the function returns the maximum value of the id
*/
int datastruct::maxId()
{
    int maxId = 0;
    if(m_point.isEmpty())
        return maxId;

    int i, len;
    len = m_point.length();

    for(i=0; i<len; ++i){
        if(m_point.at(i).idtratto > maxId)
            maxId = m_point.at(i).idtratto;
    }

    return maxId;

}


void datastruct::inverso(QPointF &point){
    point = QPointF(point.x(), point.y());
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

    double _point = point.y();
    this->scala_y(_point);
    this->scala_posizionefoglio(_point);
}

void datastruct::scala_x(double scala)
{
    int i, len;
    len = this->m_point.length();
    for(i=0; i<len; i++){
        m_point.operator[](i).m_x += scala;
    }
}

void datastruct::scala_posizionefoglio(double scala){
    int i, len;
    for(i=0, len = posizionefoglio.length(); i<len; ++i){
        posizionefoglio.operator[](i) += scala;
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

    this->zoom = 1.00;
    this->posizionefoglio.clear();
    
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
        if(max < m_point.at(i).m_x
                && m_point.at(i).idtratto != IDVERTICALE
                && m_point.at(i).idtratto != IDORIZZONALE){
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

    return (temp_id == m_point.at(i-1).idtratto
            && temp_id == m_point.at(i+1).idtratto);

}

/* max y in the list */
double datastruct::last(){
    if(m_point.isEmpty())
        return (double)0;

    double max = m_point.first().m_y;
    int i, len;
    len = m_point.length();

    for(i=0; i<len; i++){
        if(max < m_point.at(i).m_y)
            max = m_point.at(i).m_y;
    }

    return max;
}
