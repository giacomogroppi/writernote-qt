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

    void draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor& color) const override;
    void draw(WPainter &painter, bool is_rubber, int page, cdouble prop) const override;
    int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const override;
    bool is_inside(const RectF &rect, double precision) const override;

#   define stroke_append_default (-1.)
    void append(const PointF &point, pressure_t pressure) override;
    void append (WListFast<PointF> &&points, WListFast<pressure_t> &&pressures) final override;
    size_t createControl() const final override;

    RectF getBiggerPointInStroke() const override;
    bool isInside(const RectF &rect) const override;

    int save(WritableAbstract &file) const final;

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
    auto loadPtr(const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, StrokeLine*>;

    friend class StrokeLineGenerator;
    friend class Stroke;
};

