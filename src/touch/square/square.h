#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

#include "../../currenttitle/document.h"

class square
{
private:
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;
    void findObjectToDraw(Document *);

    PointSettable pointinit;
    PointSettable pointfine;

    QList<int> m_id; /* point */
    QList<int> m_index_img; /* image */

    QPen penna;

public:
    void needReload(QPainter &);

    void adjustPoint();

    square();

    inline void reset(){
        pointinit.set = lastpoint.set = pointfine.set = false;
        check = false;
        __need_reload = false;
        m_id.clear();
        m_index_img.clear();
    }

    void updatePoint(QPointF puntofine);
    bool find(Document *data);

    bool isinside(const QPointF &);
    void move(const QPointF &, Document *data);

    /* if true: it means that the user has not previously selected comething */
    bool check;

    /* definizione per i punti di spostamento */
    PointSettable lastpoint;

};

#endif // SQUARE_H
