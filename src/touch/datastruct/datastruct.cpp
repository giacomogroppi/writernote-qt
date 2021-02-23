#include "datastruct.h"
#include <QList>
#include "../method/methoddefinition.h"

datastruct::datastruct()
{

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

    /*
     * last data struct
    */
    /*
    double y_ = y.first();
    len = y.length();
    for(i=0;i<len;i++)
        if(y.at(i) > y_)
            y_ = y.at(i);
    return y_;*/
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

    /*
     * last data struct
    */
    /*
    double y_ = y.first();
    len = y.length();
    for(i=0;i<len;i++)
        if(y.at(i) > y_ && idtratto.at(i) != -1)
            y_ = y.at(i);
    return y_;*/
}

void datastruct::removeat(int i){
    m_point.removeAt(i);

    /*
     * last data struct
    */
    /*
    x.removeAt(i);
    y.removeAt(i);
    color.removeAt(i);
    idtratto.removeAt(i);
    posizioneaudio.removeAt(i);
    pressure.removeAt(i);
    rotation.removeAt(i);*/
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
