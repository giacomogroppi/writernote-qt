#pragma once

#include "StrokeNormal.h"

class StrokeForPage: public StrokeNormal {
private:
public:
    StrokeForPage();
    ~StrokeForPage() override = default;
    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const final;
};

