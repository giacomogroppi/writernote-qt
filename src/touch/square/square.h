#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

#include "../datastruct/datastruct.h"

class square
{
private:
    bool __need_reload = false;
public:
    void needReload(QPainter &);

    square();
    void reset();
    QRectF disegno(QPainter &painter, QPointF puntofine);
    bool find();

    bool isinside(QPointF);
    void setData(datastruct *data);
    QRectF move(QPointF, QPainter &painter, datastruct *data);

    /* disegna il quadrato attorno all'oggetti che ha selezionato l'utente */
    QRectF drawsquare(QPainter &painter);

    QPointF pointinit;
    QPointF pointfine;

    int idtratto;

    QPen penna;
    bool check;

    /* definizione per i punti di spostamento */
    QPointF lastpoint;

};

#endif // SQUARE_H
