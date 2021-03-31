#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

#include "../datastruct/datastruct.h"

class square
{
public:
    square();
    void reset();
    QRectF disegno(QPainter &painter, QPointF puntofine);
    bool find();

    bool isinside(QPointF);
    void setData(datastruct *data);
    QRectF move(QPointF, QPainter &painter);

    /* disegna il quadrato attorno all'oggetti che ha selezionato l'utente */
    QRectF drawsquare(QPainter &painter);

    QPointF pointinit;
    QPointF pointfine;

    datastruct *data;
    int idtratto;

    QPen penna;
    bool check;

    /* definizione per i punti di spostamento */
    QPointF lastpoint;

};

#endif // SQUARE_H
