//
// Created by Giacomo Groppi on 01/05/23.
//

#ifndef WRITERNOTE_POINTF_H
#define WRITERNOTE_POINTF_H

#include "PointTemplate.h"

/*class PointF {
private:
    double _x, _y;
public:
    explicit PointF(double x, double y);
    explicit PointF();
    ~PointF() = default;

    double x() const;
    double y() const;

    double &rx();
    double &ry();

    void setX(double x);
    void setY(double y);

    PointF& operator=(const PointF &other);
    bool operator==(const PointF &other) const;

    PointF& operator+(const PointF &other);
    PointF& operator*(double d);
    PointF& operator/(double d);

    PointF &operator*=(double d);
    PointF &operator+=(const PointF& d);
};*/

using PointF = PointTemplate<double>;

#endif //WRITERNOTE_POINTF_H
