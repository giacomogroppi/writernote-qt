#pragma once

#include "Stroke.h"

class StrokeRect final: public Stroke
{
private:
    struct StrokeRectData{
        QRectF rect;
        pressure_t press;

        bool operator==(const StrokeRectData &other) const {
            return  other.rect ==  this->rect and
                    other.press == this->press;
        }
    } _data;

    int load(WZipReaderSingle &reader);

public:
    StrokeRect();

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const;
    bool is_inside(const QRectF &rect, double precision) const;

    void append(const point_s &point, pressure_t pressure);
    size_t createControll() const final;

    QRect getBiggerPointInStroke() const;
    bool isInside(const QRectF &rect) const;

    int save(WZipWriterSingle &file) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;

    void decreasePrecision();

    void adjust(double zoom);
    std::shared_ptr<Stroke> clone() const;

    int how_much_decrese() const;

    std::shared_ptr<Stroke> makeNormal() const;

    bool isEmpty() const;

    void scale(const QPointF &offset);

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static StrokeRect *make(const QPointF& pt1, const QPointF& pt2, const class StrokeNormal &s);
    int type() const final;

    friend class StrokeRectGenerator;
    friend class Stroke;
};
