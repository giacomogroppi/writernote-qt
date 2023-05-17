#pragma once

#include "Stroke.h"

class StrokeRect final: public Stroke
{
private:
    struct StrokeRectData{
        RectF rect;
        pressure_t press;

        bool operator==(const StrokeRectData &other) const {
            return  other.rect ==  this->rect and
                    other.press == this->press;
        }
    } _data;

    int load(WZipReaderSingle &reader);

public:
    StrokeRect();

    void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const;
    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const;
    bool is_inside(const RectF &rect, double precision) const;

    void append(const PointF &point, pressure_t pressure);
    size_t createControll() const final;

    RectF getBiggerPointInStroke() const;
    bool isInside(const RectF &rect) const;

    int save(WZipWriterSingle &file) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;

    void decreasePrecision();

    void adjust(double zoom);
    std::shared_ptr<Stroke> clone() const;

    int how_much_decrese() const;

    std::shared_ptr<Stroke> makeNormal() const;

    bool isEmpty() const;

    void scale(const PointF &offset);

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static StrokeRect *make(const PointF& pt1, const PointF& pt2, const class StrokeNormal &s);
    int type() const final;

    friend class StrokeRectGenerator;
    friend class Stroke;
};
