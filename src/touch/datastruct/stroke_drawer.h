#ifndef WRITERNOTE_QT_STROKE_DRAWER_H
#define WRITERNOTE_QT_STROKE_DRAWER_H

#include "utils/common_script.h"
#include <QtGlobal>
#include <QPainter>
#include <QPen>

class stroke;

class stroke_drawer {
private:
    static void draw_stroke_normal(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);

    // complex stroke
    static void draw_circle (QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
    static void draw_rect   (QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
    static void draw_line   (QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
public:
    static void draw_stroke(QPainter &painter, const stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
};


#endif //WRITERNOTE_QT_STROKE_DRAWER_H
