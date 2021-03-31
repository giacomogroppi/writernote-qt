#ifndef SIZE_SQUARE_H
#define SIZE_SQUARE_H

#include <QRect>

class size_square
{
public:
    /*return true if the first one is bigger then the second one*/
    static bool isbigger(QRectF &first, QRectF &second);
    static bool isbigger(QRectF first, QRectF &second);
};

#endif // SIZE_SQUARE_H
