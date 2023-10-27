#pragma once

#include "Stroke.h"
#include "core/pointer/UniquePtr.h"

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

    void draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor& color) const final;
    void draw(WPainter &painter, bool is_rubber, int page, cdouble prop) const final;
    auto is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const -> int final;
    bool is_inside(const RectF &rect, double precision) const final;

#   define stroke_append_default (-1.)
    void append(const PointF &point, pressure_t pressure) final;
    void append (WListFast<PointF> &&points, WVector<pressure_t> &&pressures) final;
    auto createControl() const -> size_t final;

    auto getBiggerPointInStroke() const -> RectF final;
    auto isInside(const RectF &rect) const -> bool final;

    auto save(WritableAbstract &file) const -> int final;

    auto getSizeInMemory() const -> size_t final;
    auto getSizeInFile() const -> size_t final;

    void decreasePrecision() final;

    void adjust(double zoom) final;
    auto clone() const -> UniquePtr<Stroke> final;

    int how_much_decrese() const final;

    auto makeNormal() const -> UniquePtr<Stroke> final;

    nd
    auto isEmpty() const -> bool final;

    void scale(const PointF &offset) final;

    void preappend(int) final {};

    auto operator==(const Stroke &other) const -> bool final;
    auto operator!=(const Stroke &other) const -> bool final;

    static auto cmp(const Stroke &stroke1, const Stroke &stroke2) -> bool;
    static void copy(const Stroke &src, Stroke &dest);

    static auto make(const PointF& pt1, const PointF& pt2, const StrokeNormal &s) -> StrokeLine *;
    auto type() const -> int final;

    static
    auto loadPtr(const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, StrokeLine*>;

    friend class StrokeLineGenerator;
    friend class Stroke;
};

