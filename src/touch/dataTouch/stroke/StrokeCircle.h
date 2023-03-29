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

    double distanceFromCenter(const QPointF& point) const;
    int isInternal(double distance, double precision) const;
    bool oneSide(double inside, double outside, double prec) const;

    static constexpr bool debugCircle = false;
public:
    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const;
    bool is_inside(const QRectF &rect, double precision) const;

    void append(const Point &point, pressure_t pressure);
    virtual size_t createControll() const;

    QRect getBiggerPointInStroke() const;
    bool isInside(const QRectF &rect) const final;
    int save(WZipWriterSingle &file) const final;

    size_t getSizeInMemory() const;
    size_t getSizeInFile() const final;

    void decreasePrecision();

    void adjust(double zoom);
    std::shared_ptr<Stroke> clone() const;

    int how_much_decrese() const;

    std::shared_ptr<Stroke> makeNormal() const final;

    bool isEmpty() const final;

    void scale(const QPointF &offset) final;

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    int type() const final;

    friend class StrokeCircleGenerator;
    friend class Stroke;
};

