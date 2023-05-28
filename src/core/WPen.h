//
// Created by Giacomo Groppi on 08/05/23.
//

#ifndef WRITERNOTE_WPEN_H
#define WRITERNOTE_WPEN_H

#include "touch/dataTouch/Point.h"

#ifdef USE_QT
# include <QPen>
# include <QBrush>
#endif

class WPen {
private:
#ifdef USE_QT
    QPen _pen;
#endif // USE_QT
public:
    WPen();

#ifdef USE_QT
    void setBrush(const QBrush &brush);
#endif // USE_QT

    void setColorNull();
    void setColor(const WColor &color);
    const WColor &color() const;
    double widthF() const;

    // todo --> try to remove
    void setSolidPattern();

    void setWidthF(double value);

    enum WPenStyle {
        DotLine
    };
    void setStyle(enum WPenStyle style);
};


#endif //WRITERNOTE_WPEN_H
