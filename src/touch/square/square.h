#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

#include "../datastruct/datastruct.h"

class square
{
private:
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;
    void findObjectToDraw(datastruct *, QPointF &t_l, QPointF &b_r);

    PointSettable pointinit;
    PointSettable pointfine;

public:
    void needReload(QPainter &);

    void adjustPoint();

    square();

    inline void reset(){
        pointinit.set = lastpoint.set = pointfine.set = false;
        check = false;
        __need_reload = false;
    }

    void updatePoint(QPointF puntofine);
    bool find(datastruct *data);

    bool isinside(QPointF);
    void move(QPointF, QPainter &painter, datastruct *data);

    QList<int> m_id;

    QPen penna;

    /* if true: it means that the user has not previously selected comething */
    bool check;

    /* definizione per i punti di spostamento */
    PointSettable lastpoint;

};

#endif // SQUARE_H
