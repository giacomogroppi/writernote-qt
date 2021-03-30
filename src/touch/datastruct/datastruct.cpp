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

    const point_s * __point = &m_point.first();
    for(int i=0, len = m_point.length(); i<len; i++){
        if(__point->idtratto < _minId){
            _minId = __point->idtratto;
        }
    }

    return _minId;
}

/*
 * return true if the
 * point is write by user
*/
bool datastruct::isIdUser(const point_s *__point)
{
    return __point->idtratto != IDTRATTOZERO
            && __point->idtratto != IDVERTICALE
            && __point->idtratto != IDORIZZONALE;
}

bool datastruct::isinside(QPointF &topleft, QPointF &bottonright, unsigned int index)
{
    return isinside(topleft.x(),
                    topleft.y(),
                    bottonright.x(),
                    bottonright.y(),
                    index);
}

bool datastruct::isinside(double x1, double y1, double x2, double y2, unsigned int index)
{
    const point_s *__point;

    __point = &m_point.at(index);

    return __point->m_x > x1
            && __point->m_y > y1
            && __point->m_x < x2
            && __point->m_y < y2
            && isIdUser(__point) ;
}

void datastruct::adjustHeight(int height)
{
    point_s & __point = m_point.first();
    QPointF _traslation_point(0.0, 0.0);

    if(__point.m_x > 0.0){
        _traslation_point.setX(__point.m_x);
    }

    if(__point.m_y > 0.0){
        _traslation_point.setY(__point.m_y);
    }

    if(_traslation_point != QPointF(0.0, 0.0)){
        datastruct::inverso(_traslation_point);
        scala_all(_traslation_point);
    }

    double _maxy = biggery();

    if(_maxy < height)
        scala_y((height-_maxy));
}

unsigned int datastruct::posIdOrizzonal()
{
    unsigned int i, len;
    len = m_point.length();

    for(i=0; i<len; ++i){
        if(m_point.at(i).idtratto == IDORIZZONALE)
            return i;
    }

    return 0;
}

datastruct::datastruct()
{

}

static void reorganize_p(datastruct *data, int index, unsigned int len){

}

/* the function restores the ids, as
 * there may be holes, when we go
 * to eliminate some strokes written
 * with the pen
*/
void datastruct::reorganize()
{
    int i, len, _lastId, secondI;
    len = m_point.length();

    if(!len)
        return;

    secondI = minId();

    const point_s * __point = &m_point.first();

    _lastId = __point->idtratto;

    for(i=0; i<len; ++i){
        __point = &m_point.at(i);
        if(__point->idtratto != secondI+1){

        }
    }

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
    point *= -1.0;
}

void datastruct::repositioning()
{
    if(isempty())
        return;

    point_s & __point = m_point.first();

    if(__point.m_x == 0.0 && __point.m_y == 0)
        return;

    QPointF point_temp(__point.m_x,__point.m_y);


    datastruct::inverso(point_temp);

    this->scala_all(point_temp);

}

QPointF datastruct::scala_all()
{
    if(this->m_point.isEmpty())
        return QPointF(0, 0);

    point_s & __point = m_point.first();

    QPointF point(__point.m_x, __point.m_y);

    datastruct::inverso(point);

    scala_all(point);

    return point;
}

void datastruct::scala_all(QPointF &point)
{
    if(point == QPointF(0, 0))
        return;

    this->scala_x(point.x());

    double _point = point.y();
    this->scala_y(_point);
    this->scala_posizionefoglio(_point);
}

void datastruct::scala_x(double scala)
{
    unsigned int i, len;
    len = this->m_point.length();
    for(i=0; i<len; i++){
        m_point.operator[](i).m_x += scala;
    }
}

void datastruct::scala_posizionefoglio(double scala){
    unsigned int i, len;
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

    unsigned int i, len;
    double y_;
    const point_s * __point = &m_point.first();

    y_ = __point->m_y;
    len = m_point.length();

    for(i=0; i<len; i++){
        __point = &m_point.at(i);

        if(__point->m_y > y_)
            y_ = __point->m_y;
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


auto datastruct::biggerynoid() -> double{
    if(m_point.isEmpty())
        return (double)0;

    int i, len, temp;
    double y_ = m_point.first().m_y;
    len = m_point.length();

    for(i=0; i<len; i++){
        if(m_point.at(i).m_y > y_){
            temp = m_point.at(i).idtratto;
            if(temp != IDVERTICALE
                    && temp != IDORIZZONALE
                    && temp != IDTRATTOZERO){
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
    if(!len)
        return 0;

    const point_s * __point = &m_point.first();


    max = __point->m_x;


    for(i=0; i<len; i++){
        __point = &m_point.at(i);

        if(max < __point->m_x
                && __point->idtratto != IDVERTICALE
                && __point->idtratto != IDORIZZONALE
                && __point->idtratto != IDTRATTOZERO){
            max = __point->m_x;
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


    double max;
    int i, len;
    len = m_point.length();

    const point_s * __point = &m_point.first();
    max = __point->m_y;

    for(i=0; i<len; i++){
        __point = &m_point.at(i);

        if(max < __point->m_y){
            max = __point->m_y;
        }
    }

    return max;
}
