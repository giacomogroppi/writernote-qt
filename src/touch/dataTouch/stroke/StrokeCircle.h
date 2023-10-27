#pragma once

#include "Stroke.h"
#include "touch/dataTouch/stroke/StrokeNormal.h"

class StrokeCircle final: public Stroke
{
private:
    struct StrokeCircleData {
        PointF position;
        double r;
        pressure_t press;

        bool operator==(const StrokeCircleData &other) const = default;
    } _data;

    int load(WZipReaderSingle &reader);

    StrokeCircle();

    double distanceFromCenter(const PointF& point) const;
    int isInternal(double distance, double precision) const;
    bool oneSide(double inside, double outside, double prec) const;

    static constexpr bool debugCircle = false;
public:
    void draw(WPainter &painter, bool is_rubber, int page, double fakeProp, const WColor& color) const override;
    void draw(WPainter &painter, bool is_rubber, int page, cdouble prop) const override;

    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const override;
    bool is_inside(const RectF &rect, double precision) const override;

    void append(const PointF &point, pressure_t pressure) override;
    void append (WListFast<PointF> &&points, WVector<pressure_t> &&pressures) final;
    virtual size_t createControl() const override;

    auto getBiggerPointInStroke() const -> RectF final;
    auto isInside(const RectF &rect) const -> bool final;
    auto save(WritableAbstract &file) const -> int final;

    auto getSizeInMemory() const -> size_t final;
    auto getSizeInFile() const -> size_t final;

    void decreasePrecision() final;

    void adjust(double zoom) final;
    auto clone() const -> UniquePtr<Stroke> final;

    auto how_much_decrese() const -> int final;

    auto makeNormal() const -> UniquePtr<Stroke> final;

    auto isEmpty() const -> bool final;

    void scale(const PointF &offset) final;

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    int type() const final;

    static
    auto loadPtr (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, StrokeCircle*>;

    friend class StrokeCircleGenerator;
    friend class Stroke;
};

