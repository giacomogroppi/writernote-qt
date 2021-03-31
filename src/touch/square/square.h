#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

class datastruct;

class square
{
private:
    bool __need_reload = false;

    QRectF findObjectToDraw(datastruct *);

public:
    void needReload(QPainter &);

    square();
    void reset();
    QRectF disegno(QPainter &painter, QPointF puntofine);
    bool find(datastruct *data);

    bool isinside(QPointF);
    QRectF move(QPointF, QPainter &painter, datastruct *data);

    /* disegna il quadrato attorno all'oggetti che ha selezionato l'utente */
    QRectF drawsquare(QPainter &painter, datastruct *data);

    QPointF pointinit;
    QPointF pointfine;

    QList<int> m_id;

    QPen penna;
    bool check;

    /* definizione per i punti di spostamento */
    QPointF lastpoint;

};

#endif // SQUARE_H
