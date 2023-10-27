#pragma once

#include "Stroke.h"
#include "core/pointer/UniquePtr.h"

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

    void draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor& color) const override;
    void draw(WPainter &painter, bool is_rubber, int page, double prop) const override;

    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const final;
    bool is_inside(const RectF &rect, double precision) const final;

    void append(const PointF &point, pressure_t pressure) final;
    void append (WListFast<PointF> &&points, WVector<pressure_t> &&pressures) final;
    auto createControl() const -> size_t final;

    auto getBiggerPointInStroke() const -> RectF final;
    bool isInside(const RectF &rect) const final;

    int save(WritableAbstract &file) const final;

    auto getSizeInMemory() const -> size_t final;
    auto getSizeInFile() const -> size_t final;

    void decreasePrecision() final;

    void adjust(double zoom) final;
    auto clone() const -> UniquePtr<Stroke> final;

    int how_much_decrese() const final;

    auto makeNormal() const -> UniquePtr<Stroke> final;

    auto isEmpty() const -> bool final;

    void scale(const PointF &offset) final;

    void preappend(int) final {};

    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    static StrokeRect *make(const PointF& pt1, const PointF& pt2, const class StrokeNormal &s);
    int type() const final;

    static
    auto loadPtr (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, StrokeRect*>;

    friend class StrokeRectGenerator;
    friend class Stroke;
};
