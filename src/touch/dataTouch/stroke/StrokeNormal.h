#pragma once

#include "StrokeTmp.h"
#include <QList>

class StrokeNormal: public Stroke{
private:
    int save(WZipWriterSingle &file) const final;
    int load(WZipReaderSingle &reader, int version);

    QList<point_s> _point;
    QList<pressure_t> _pressure;
    [[nodiscard]] static QRect getBiggerPointInStroke(QList<point_s>::const_iterator begin, QList<point_s>::const_iterator end, StrokePre s);
    [[nodiscard]] auto length () const { return _point.length(); }
public:
    StrokeNormal();
    ~StrokeNormal();

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const final;
    [[nodiscard]] int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const final;

#   define stroke_append_default (-1.)
    void append(const point_s &point, pressure_t pressure) final;
    [[nodiscard]] size_t createControll() const final;

    [[nodiscard]] QRect getBiggerPointInStroke() const final;
    [[nodiscard]] bool isInside(const QRectF &rect) const;

    [[nodiscard]] const struct metadata_stroke &getMetadata() const;

    [[nodiscard]] bool is_highlighter() const final;
    [[nodiscard]] size_t getSizeInMemory() const final;
    [[nodiscard]] size_t getSizeInFile() const final;
    void decreasePrecision() final;

    /* this function physically adds the x and y value of the point to all of its points. */
    void movePoint(const QPointF &translation) final;

    void reset() final;
    Stroke &operator=(const Stroke &other);

    [[nodiscard]] bool isEmpty() const final;

    void scale(const QPointF &offset) final;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    friend class Page;
    friend class xmlstruct;
    friend class stroke_drawer;
    friend class stroke_file;
    friend class page_file;
    friend void stroke_complex_adjust(Stroke *stroke, cdouble zoom);
};


