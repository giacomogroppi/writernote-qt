#pragma once

#include <QList>
#include <QDebug>
#include <QRect>
#include <QPainterPath>
#include "touch/dataTouch/point.h"
#include "utils/common_def.h"
#include "utils/WCommonScript.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "core/WLine.h"
#include "stroke_complex_data.h"
#include "touch/object_finder/model/model.h"
#include "testing/memtest.h"
#include "core/WZipWriterSingle.h"
#include "core/WZipReaderSingle.h"
#include "core/WImage.h"
#include "StrokeProp.h"

struct metadata_stroke{
    int posizione_audio;
    struct colore_s color;
};

class Stroke
{
private:
    struct metadata_stroke _metadata;

    QRect _biggerData;

    enum flag_status : unsigned char{
        UPDATE_BIGGER_DATA = BIT(1),
        UPDATE_PRESSURE = BIT(2)
    };

    unsigned char _flag;

    [[nodiscard]] bool isPressureVal() const;
    [[nodiscard]] bool needToUpdatePressure() const;

    void setFlag(unsigned char type, bool value) const;

    static_assert(sizeof(_flag) * 8 >= 4 );

    void updateFlagPressure() const;

    virtual int save(WZipWriterSingle &file) const = 0;
    Stroke* load(WZipReaderSingle &reader, int version);

public:
    ~Stroke();

    virtual void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const = 0;
    [[nodiscard]] virtual int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const = 0;

    [[nodiscard]] QColor getColor(double division) const;

#   define stroke_append_default (-1.)
    virtual void append(const point_s &point, pressure_t pressure) = 0;
    void setMetadata(int posizione_audio, const colore_s &color);
    virtual void setMetadata(const metadata_stroke &metadata) final;
    void setPositioneAudio(int m_pos_ris);
    [[nodiscard]] virtual size_t createControll() const;

    [[nodiscard]] virtual int getPosizioneAudio() const = 0;
    [[nodiscard]] virtual QRect getBiggerPointInStroke() const = 0;
    [[nodiscard]] virtual bool isInside(const QRectF &rect) const = 0;

    virtual void clearAudio() = 0;

    [[nodiscard]] const struct metadata_stroke &getMetadata() const;

    [[nodiscard]] virtual bool is_highlighter() const = 0;
    [[nodiscard]] uchar get_alfa() const;
    [[nodiscard]] virtual size_t getSizeInMemory() const = 0;
    [[nodiscard]] virtual size_t getSizeInFile() const = 0;
    virtual void decreasePrecision() = 0;
    void setAlfaColor(uchar alfa);

    virtual void setColor(const colore_s &color) final;
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

protected:
    Stroke();
    Stroke(const metadata_stroke& met);
    void modify();
    void setBiggerData(const QRect &newRect) const;
    [[nodiscard]] bool needToUpdateBiggerData() const;
    /* all stroke derivated class needs to implements this method to recognize yourself */
    virtual int type() = 0;
    virtual void preappend(int l) = 0;

};

inline void Stroke::setBiggerData(const QRect &newRect) const
{
    auto &r = (QRect &) this->_biggerData;
    r = newRect;
    setFlag(UPDATE_BIGGER_DATA, false);
}
