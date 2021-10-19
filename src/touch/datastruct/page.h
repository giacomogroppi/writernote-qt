#ifndef PAGE_H
#define PAGE_H

#include <QList>
#include <QPointF>
#include "point.h"

class page
{
private:
    const uint height = NUMEROPIXELVERTICALI;
    const uint width = NUMEROPIXELORIZZONALI;
    bool IsVisible = true;
    int count;
    QList<point_s> m_point;
public:
    page(int count);
    static int getHeight();
    void updateFlag(const QPointF &FirstPoint);
    const point_s * at(const uint i) const;
    point_s * at_mod(const uint i);
    uint length() const;
    bool isVisible() const;
    static void copy(const page &src, page &dest);
    void removeAt(const uint i);
    int maxId() const;
    const point_s * last() const;
};

int page::getHeight(){
    return NUMEROPIXELORIZZONALI;
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

#endif // PAGE_H
