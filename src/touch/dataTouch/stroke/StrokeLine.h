#pragma once

#include "StrokeTmp.h"

class StrokeLine : Stroke
{
private:
    QPointF _pt1, _pt2;
    pressure_t _press;
public:
    StrokeLine();

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const;
    [[nodiscard]] int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const;

    [[nodiscard]] QColor getColor(double division) const;

#   define stroke_append_default (-1.)
    virtual void append(const point_s &point, pressure_t pressure) = 0;
    void setMetadata(int posizione_audio, const colore_s &color);
    void setMetadata(const metadata_stroke &metadata);
    void setPositioneAudio(int m_pos_ris);
    [[nodiscard]] virtual size_t createControll() const;

    [[nodiscard]] int getPosizioneAudio() const;
    [[nodiscard]] virtual QRect getBiggerPointInStroke() const = 0;
    [[nodiscard]] virtual bool isInside(const QRectF &rect) const = 0;

    void clearAudio();

    [[nodiscard]] const struct metadata_stroke &getMetadata() const;

    [[nodiscard]] bool is_highlighter() const;
    [[nodiscard]] uchar get_alfa() const;
    [[nodiscard]] virtual size_t getSizeInMemory() const = 0;

    [[nodiscard]] virtual size_t getSizeInFile() const;
    virtual void decreasePrecision() = 0;
    void setAlfaColor(uchar alfa);

    void setColor(const colore_s &color);
    /* this function physically adds the x and y value of the point to all of its points. */
    virtual void movePoint(const QPointF &translation) = 0;

    virtual void reset();

    virtual Stroke *clone() const;

    [[nodiscard]] virtual bool isEmpty() const = 0;

    virtual void scale(const QPointF &offset) = 0;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

private:
    int type() const final;
    void preappend(int) final {};
};

