#pragma once

#include "Stroke.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

class StrokeCircle final: public Stroke
{
private:
    struct StrokeCircleData{
        double x, y, r;
        pressure_t press;

        bool operator==(const StrokeCircleData &other) const {
            return  this->press == other.press and
                    this->x == other.x and
                    this->y == other.y and
                    this->r == other.r;
        }
    } _data;

    int load(WZipReaderSingle &reader);

    StrokeCircle();

    double distanceFromCenter(const PointF& point) const;
    int isInternal(double distance, double precision) const;
    bool oneSide(double inside, double outside, double prec) const;

    static constexpr bool debugCircle = false;
public:
    void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const;
    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const;
    bool is_inside(const RectF &rect, double precision) const;

    void append(const PointF &point, pressure_t pressure);
    virtual size_t createControll() const;

    RectF getBiggerPointInStroke() const;
    bool isInside(const RectF &rect) const final;
    int save(WZipWriterSingle &file) const final;

    size_t getSizeInMemory() const;
    size_t getSizeInFile() const final;

    void decreasePrecision();

    void adjust(double zoom);
    std::shared_ptr<Stroke> clone() const;

    int how_much_decrese() const;

    std::shared_ptr<Stroke> makeNormal() const final;

    bool isEmpty() const final;

    void scale(const PointF &offset) final;

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    int type() const final;

    friend class StrokeCircleGenerator;
    friend class Stroke;
};

