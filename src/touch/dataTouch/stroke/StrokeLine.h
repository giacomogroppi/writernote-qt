#pragma once

#include "Stroke.h"

class StrokeLine final : public Stroke
{
private:
    struct StrokeLineData{
        QPointF pt1, pt2;
        pressure_t press;
    } _data;

    void makeNormalVertical(class StrokeNormal *mergeTo, int from, int to) const;
    void makeNormalGeneric(class StrokeNormal *mergeTo, int from, int to) const;

    int load(WZipReaderSingle &reader);

    static constexpr bool debugLine = false;

public:
    StrokeLine();

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
    int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const;
    bool is_inside(const QRectF &rect, double precision) const;

#   define stroke_append_default (-1.)
    void append(const point_s &point, pressure_t pressure);
    size_t createControll() const final;

    QRect getBiggerPointInStroke() const;
    bool isInside(const QRectF &rect) const;

    int save(WZipWriterSingle &file) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;

    void decreasePrecision();

    void adjust(double zoom);
    std::shared_ptr<Stroke> clone() const final;

    int how_much_decrese() const;

    std::shared_ptr<Stroke> makeNormal() const;

    [[nodiscard]] bool isEmpty() const;

    void scale(const QPointF &offset);

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    static StrokeLine *make(const QPointF& pt1, const QPointF& pt2, const StrokeNormal &s);
    int type() const final;


    friend class StrokeLineGenerator;
    friend class Stroke;
};

