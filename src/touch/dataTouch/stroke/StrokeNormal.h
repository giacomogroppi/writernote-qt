#pragma once

#include "StrokeTmp.h"

class StrokeNormal: public Stroke{
    int save(WZipWriterSingle &file) const final;
    int load(WZipReaderSingle &reader, int version) final;

public:
    StrokeNormal();
    ~StrokeNormal();

    virtual void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const = 0;
    [[nodiscard]] virtual int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const = 0;

#   define stroke_append_default (-1.)
    virtual void append(const point_s &point, pressure_t pressure) = 0;
    void setMetadata(int posizione_audio, const colore_s &color);
    void setPositioneAudio(int m_pos_ris);
    [[nodiscard]] virtual size_t createControll() const = 0;

    [[nodiscard]] virtual int getPosizioneAudio() const = 0;
    [[nodiscard]] QRect getBiggerPointInStroke() const;
    [[nodiscard]] virtual bool isInside(const QRectF &rect) const = 0;

    virtual void clearAudio() = 0;

    [[nodiscard]] const struct metadata_stroke &getMetadata() const;

    [[nodiscard]] virtual bool is_highlighter() const = 0;
    [[nodiscard]] virtual uchar get_alfa() const final;
    [[nodiscard]] virtual size_t getSizeInMemory() const = 0;
    [[nodiscard]] virtual size_t getSizeInFile() const = 0;
    virtual void decreasePrecision() = 0;
    void setAlfaColor(uchar alfa);

    /* this function physically adds the x and y value of the point to all of its points. */
    virtual void movePoint(const QPointF &translation) = 0;

    virtual void reset() = 0;
    Stroke &operator=(const Stroke &other);

    [[nodiscard]] virtual bool isEmpty() const = 0;

    virtual void scale(const QPointF &offset) = 0;

    static bool cmp(const Stroke &stroke1, const Stroke &stroke2);
    static void copy(const Stroke &src, Stroke &dest);

    friend class Page;
    friend class xmlstruct;
    friend class stroke_drawer;
    friend class stroke_file;
    friend class page_file;
    friend void stroke_complex_adjust(Stroke *stroke, cdouble zoom);
};

