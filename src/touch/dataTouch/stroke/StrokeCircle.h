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

        bool operator==(const StrokeCircleData &other) const {
            return  this->press == other.press and
                    this->position == other.position and
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
    void draw(WPainter &painter, bool is_rubber, int page, double fakeProp, const WColor& color) const override;
    void draw(WPainter &painter, bool is_rubber, int page, cdouble prop) const override;

    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const override;
    bool is_inside(const RectF &rect, double precision) const override;

    void append(const PointF &point, pressure_t pressure) override;
    void append (WListFast<PointF> &&points, WListFast<pressure_t> &&pressures) final;
    virtual size_t createControl() const override;

    RectF getBiggerPointInStroke() const override;
    bool isInside(const RectF &rect) const final;
    int save(WritableAbstract &file) const final;

    size_t getSizeInMemory() const override;
    size_t getSizeInFile() const final;

    void decreasePrecision() override;

    void adjust(double zoom) override;
    std::unique_ptr<Stroke> clone() const override;

    int how_much_decrese() const override;

    std::unique_ptr<Stroke> makeNormal() const final;

    bool isEmpty() const final;

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

