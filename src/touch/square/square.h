#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

class datastruct;

class square
{
private:
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;
    QRectF findObjectToDraw(datastruct *);

    QPointF pointinit;
    QPointF pointfine;

public:
    void needReload(QPainter &);

    void adjustPoint();

    square();
    void reset();
    QRectF updatePoint(QPointF puntofine);
    bool find(datastruct *data);

    bool isinside(QPointF);
    QRectF move(QPointF, QPainter &painter, datastruct *data);

    QList<int> m_id;

    QPen penna;

    /* if true: it means that the user has not previously selected anything */
    bool check;

    /* definizione per i punti di spostamento */
    QPointF lastpoint;

};

#endif // SQUARE_H
