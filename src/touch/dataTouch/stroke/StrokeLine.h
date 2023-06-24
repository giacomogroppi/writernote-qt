#pragma once

#include "Stroke.h"

class StrokeLine final : public Stroke
{
private:
    struct StrokeLineData{
        PointF pt1, pt2;
        pressure_t press;
    } _data;

    void makeNormalVertical(class StrokeNormal *mergeTo, int from, int to) const;
    void makeNormalGeneric(class StrokeNormal *mergeTo, int from, int to) const;

    int load(WZipReaderSingle &reader);

    static constexpr bool debugLine = false;

public:
    StrokeLine();

    void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const;
    int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const;
    bool is_inside(const RectF &rect, double precision) const;

#   define stroke_append_default (-1.)
    void append(const PointF &point, pressure_t pressure);
    size_t createControll() const final;

    RectF getBiggerPointInStroke() const;
    bool isInside(const RectF &rect) const;

    int save(WZipWriterSingle &file) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;

    void decreasePrecision();

    void adjust(double zoom);
    std::unique_ptr<Stroke> clone() const final;

    int how_much_decrese() const final;

    std::unique_ptr<Stroke> makeNormal() const final;

    [[nodiscard]] bool isEmpty() const final;

    void scale(const PointF &offset) final;

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    static StrokeLine *make(const PointF& pt1, const PointF& pt2, const StrokeNormal &s);
    int type() const final;

    static
    auto loadPtr(const VersionFileController &versionController, ReadableAbstract &readable) -> std::pair<int, StrokeLine*>;

    friend class StrokeLineGenerator;
    friend class Stroke;
};

