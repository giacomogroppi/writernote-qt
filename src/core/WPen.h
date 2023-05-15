//
// Created by Giacomo Groppi on 08/05/23.
//

#ifndef WRITERNOTE_WPEN_H
#define WRITERNOTE_WPEN_H

#include "touch/dataTouch/Point.h"

#ifdef USE_QT
# include <WPen>
#endif

class WPen {
#ifdef USE_QT
    WPen _pen;
#endif // USE_QT
public:
    WPen();
    void setColorNull();
    void setColor(const colore_s &color);
    const colore_s &color() const;
    double widthF() const;

    // todo --> try to remove
    void setSolidPattern();

    void setWidthF(double value);
};


#endif //WRITERNOTE_WPEN_H
