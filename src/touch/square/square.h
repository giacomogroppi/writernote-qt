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
    QRect disegno(QPainter &painter);
    bool find();

    void setData(datastruct *data);

    /* disegna il quadrato attorno all'oggetti che ha selezionato l'utente */
    QRect drawsquare(QPainter &painter);

    QPoint pointinit;
    QPoint pointfine;

    datastruct *data;
    int idtratto;

    QPen penna;
    bool check;
};

#endif // SQUARE_H
