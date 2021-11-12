#ifndef ITSPOSSIBLETOSCROLL_H
#define ITSPOSSIBLETOSCROLL_H

#include <QSize>
#include <QPointF>
class datastruct;

class scroll{

public:
    static bool itspossibletoscrolly(const datastruct *data,
                                     const short int height,
                                     double &__pos_delta);
    static bool itspossibletoscrollx(const datastruct *data,
                                     const short int width,
                                     double &__pos_delta);

    static bool itspossibletoscroll(const datastruct &data,
                                     const QSize &size,
                                     QPointF &__pos_delta);

};

#endif // ITSPOSSIBLETOSCROLL_H
