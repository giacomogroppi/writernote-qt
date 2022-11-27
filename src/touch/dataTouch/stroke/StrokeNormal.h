#pragma once

#include "Stroke.h"
#include <QList>

class StrokeNormal final: public Stroke{
private:
    int save(WZipWriterSingle &file) const final;
    int load(WZipReaderSingle &reader, int version);

    QList<point_s> _point;
    QList<pressure_t> _pressure;
    [[nodiscard]] static QRect getBiggerPointInStroke(QList<point_s>::const_iterator begin, QList<point_s>::const_iterator end);
    bool isInsideBiggerData(const QRect &rect) const;
    int removeAt(int i);
    int type() const final;
public:
    StrokeNormal(const StrokeNormal &ref);
    StrokeNormal();
    ~StrokeNormal() final;

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const override;
    [[nodiscard]] int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const final;

#   define stroke_append_default (-1.)
    void append(const point_s &point, pressure_t pressure) final;
    [[nodiscard]] size_t createControll() const final;

    [[nodiscard]] QRect getBiggerPointInStroke() const final;
    [[nodiscard]] bool isInside(const QRectF &rect) const final;

    [[nodiscard]] size_t getSizeInMemory() const final;
    [[nodiscard]] size_t getSizeInFile() const final;
    void decreasePrecision() final;

    /* this function physically adds the x and y value of the point to all of its points. */
    void movePoint(const QPointF &translation) final;

    /**
     * This function will remove the point belonging [index, length())
     * */
    StrokeNormal *split(int index);

    [[nodiscard]] Stroke *clone() const final;

    void preappend(int i) final;

    [[nodiscard]] bool isEmpty() const final;
    void adjust(double zoom) final;
    void scale(const QPointF &offset) final;

    void force_pressure(pressure_t press);

    Stroke *makeNormal() const final;

    static bool cmp(const StrokeNormal &stroke1, const StrokeNormal &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    StrokeNormal& operator=(const StrokeNormal &other);

protected:
    auto length () const { return _point.length(); }
    auto getPressure() const{
        W_ASSERT(this->_pressure.length());
        W_ASSERT(this->_pressure.at(0) >= 0.);
        return this->_pressure.at(0);
    }

#ifdef DEBUGINFO
    friend class page_file;
#endif // DEBUGINFO

    friend class StrokeNormalFileSave;
    friend class StrokeNormalFileLoad;
    friend class Page;
    friend class StrokeForPage;
    friend class stroke_drawer;
    friend class stroke_file;
    friend class page_file;
    friend void stroke_complex_adjust(Stroke *stroke, cdouble zoom);
};

inline Stroke *StrokeNormal::makeNormal() const
{
    return nullptr;
}


