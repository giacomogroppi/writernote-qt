#ifndef PAGE_H
#define PAGE_H

#include <QList>
#include <QPointF>
#include <QDebug>
#include <QImage>
#include "point.h"

enum n_style: int;

class page
{
private:
    static const uint width = NUMEROPIXELORIZZONALI;
    static const uint height = width*1.4141; // correct proportions for A4 paper size

    bool IsVisible = true;
    int count;

    QList<point_s> m_point;
    QList<point_s> tmp;

    void drawNewPage(n_style __style);

    QImage imgDraw;
    void mergeList();
    void drawEngine(QPainter &painter, QList<point_s> &List,
                    const int len, int i, const bool is_play, const int m_pos_ris);
    void draw(QPainter &painter, const int m_pos_ris, const bool is_play, const bool all);

    point_s *at_translation(const QList<point_s> &point, uint index);

public:
    const QImage &getImg() const;

    page(const int count, const n_style style);

    static double getHeight();
    static double getWidth();

    static double getResolutionWidth();
    static double getResolutionHeigth();


    void updateFlag(const QPointF &FirstPoint, const double zoom, const double heightView);
    void setVisible(const bool vis){this->IsVisible = vis;}

    const point_s       * at(const uint i) const;
    point_s             * at_mod(const uint i);
    void at_draw(const uint i, const QPointF &translation, point_s &point, const double zoom) const;

    uint length() const;
    bool isVisible() const;
    static void copy(const page &src, page &dest);
    void removeAt(const uint i);
    int maxId() const;
    const point_s * last() const;

    /*
     * these two functions do not automatically launch
     *  the drawing of the whole sheet, they wait for
     *  the triggerRenderImage to be executed.
    */
    void append(const point_s &point);
    void append(const point_s *point);

    double minHeight() const;
    double currentHeight() const;
    double currentWidth() const;

    void changeId(const uint i, const int newId);
    bool needtochangeid(const uint index) const;
    bool userWrittenSomething() const;
    void move(const uint from, const uint to);

    void triggerRenderImage(int m_pos_ris, const bool is_play, const bool all);
};

inline double page::currentHeight() const
{
    return count*page::getHeight();
}

inline double page::currentWidth() const
{
    return page::getWidth();
}

inline void page::move(const uint from, const uint to)
{
    this->m_point.move(from, to);
}

inline point_s *page::at_translation(const QList<point_s> &point, uint index)
{
    static point_s tmp;
    //const double xtranslation = (this->count-1)*page::getWidth();
    const double ytranslation = (this->count-1)*page::getHeight();

    memcpy(&tmp, &point.at(index), sizeof(point_s));
    //tmp.m_x -= xtranslation;
    tmp.m_y -= ytranslation;
    return &tmp;
}

inline const QImage &page::getImg() const
{
    return this->imgDraw;
}

inline double page::getHeight(){
    return height;
}

inline double page::getWidth()
{
    return width;
}

inline double page::getResolutionWidth()
{
    return getWidth()*5;
}

inline double page::getResolutionHeigth()
{
    return getHeight()*5;
}

inline void page::updateFlag(const QPointF &FirstPoint, const double zoom, const double heightView)
{
    static double heightSec;
    static double translation;

    heightSec = double(height)*zoom;
    translation = -FirstPoint.y();

    IsVisible =  ((heightSec*count) >= translation) && ((heightSec*(count-1)) <= translation);
    IsVisible = (IsVisible) ? IsVisible : (count-1)*heightSec <= heightView && (count-1)*heightSec >= translation;
    IsVisible = true;
}

inline const point_s *page::at(uint i) const
{
    return &this->m_point.at(i);
}

inline point_s *page::at_mod(uint i)
{
    return &this->m_point.operator[](i);
}

inline void page::at_draw(const uint i, const QPointF &translation, point_s &point, const double zoom) const
{
    memcpy(&point, at(i), sizeof(point_s));

    point.m_x *= zoom;
    point.m_y *= zoom;

    point.m_x += translation.x();
    point.m_y += translation.y();
}

inline uint page::length() const
{
    return m_point.length();
}

inline bool page::isVisible() const
{
    return this->IsVisible;
}

inline void page::copy(const page &src, page &dest){
    dest.m_point = src.m_point;
}

inline void page::removeAt(const uint i)
{
    this->m_point.removeAt(i);
}

inline int page::maxId() const
{
    static uint i;
    static uint len;
    static int id;
    static const point_s *point;

    len = length();
    id = 0;

    for(i = 0; i < len; i++){
        point = at(i);

        if(point->idtratto > id)
            id = point->idtratto;
    }

    return id;
}

inline const point_s *page::last() const
{
    return &this->m_point.last();
}

inline void page::append(const point_s &point)
{
    this->tmp.append(point);
    //this->m_point.append(point);
}

inline void page::append(const point_s *point)
{
    this->append(*point);
}

inline double page::minHeight() const
{
    return (this->count-1)*this->height;
}

#endif // PAGE_H
