#pragma once

#include "utils/common_script.h"
#include <QtGlobal>
#include <QPainter>
#include <QPen>

class Stroke;

class stroke_drawer {
private:
    static void draw_stroke_normal(QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);

    // complex stroke
    static void draw_circle (QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
    static void draw_rect   (QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
    static void draw_line   (QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
public:
    static void draw_stroke(QPainter &painter, const Stroke &stroke, cint page, QPen &pen, cbool is_rubber, cdouble prop);
};

