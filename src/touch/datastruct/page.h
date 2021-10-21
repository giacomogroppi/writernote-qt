#ifndef PAGE_H
#define PAGE_H

#include <QList>
#include <QPointF>
#include "point.h"

enum n_style: int;

class page
{
private:
    static const uint height = NUMEROPIXELVERTICALI;
    static const uint width = height*1.4141; // correct proportions for A4 paper size
    bool IsVisible = true;
    int count;
    QList<point_s> m_point;
    void drawNewPage(n_style __style);

public:
    page(const int count, const n_style style);
    static int getHeight();
    static int getWidth();
    void updateFlag(const QPointF &FirstPoint);
    const point_s * at(const uint i) const;
    point_s * at_mod(const uint i);
    uint length() const;
    bool isVisible() const;
    static void copy(const page &src, page &dest);
    void removeAt(const uint i);
    int maxId() const;
    const point_s * last() const;
    double biggerynoid() const;

    void append(const point_s &point);
    void append(const point_s *point);

    int currentHeight() const;
    int currentWidth() const;

    void changeId(const uint i, const int newId);
    bool needtochangeid(const uint index) const;
    bool userWrittenSomething() const;
    void move(const uint from, const uint to);
};

int page::currentHeight() const
{
    return count*page::getHeight();
}

int page::currentWidth() const
{
    return page::getWidth();
}

int page::getHeight(){
    return height;
}

inline int page::getWidth()
{
    return width;
}

void page::updateFlag(const QPointF &FirstPoint){
    IsVisible = true;

    if((-FirstPoint.y()) > count*height)
        IsVisible = false;
    if((-FirstPoint.y() + height < height*count))
        IsVisible = false;
}

const point_s *page::at(uint i) const{
    return &this->m_point.at(i);
}

point_s *page::at_mod(uint i){
    return &this->m_point.operator[](i);
}

uint page::length() const{
    return m_point.length();
}

bool page::isVisible() const{
    return this->IsVisible;
}

void page::copy(const page &src, page &dest){
    dest.m_point = src.m_point;
}

inline void page::removeAt(const uint i)
{
    this->m_point.removeAt(i);
}

/*inline double page::biggerynoid() const
{
    uint i;
    const uint len = this->m_point.length();
    double max = 0.0;

    for(i=0; i<len; i++){
        if(this->m_point.at(i).m_y > max)
            max = this->m_point.at(i).m_y;
    }
    return max;
}*/

#endif // PAGE_H
