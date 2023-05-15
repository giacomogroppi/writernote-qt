#pragma once

#include <memory>
#include "utils/WCommonScript.h"
#include "touch/dataTouch/Point.h"
#include "utils/common_def.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "core/WLine.h"
#include "testing/memtest.h"
#include "core/WZipWriterSingle.h"
#include "core/WZipReaderSingle.h"
#include "core/WImage.h"
#include "core/WPainter/WPainter.h"
#include "core/Point.h"
#include "core/WPen.h"

#define PROP_RESOLUTION (2.)
#define SetRenderPainter(painter) painter.setRenderHint(WPainter::Antialiasing, true);
constexpr double deltaPress = 2.;
constexpr double deltaColorNull = 1.3;

struct metadata_stroke{
    int posizione_audio;
    struct colore_s color;

    bool operator!=(const metadata_stroke &other) const;
    bool operator==(const metadata_stroke &other) const;

    metadata_stroke &operator=(const metadata_stroke &other)
    {
        this->color = other.color;
        this->posizione_audio = other.posizione_audio;
        return *this;
    }
};

class Stroke
{
private:
    struct metadata_stroke _metadata;
    mutable RectF _biggerData;

    enum flag_status : unsigned char{
        UPDATE_BIGGER_DATA = BIT(1)
    };

    mutable unsigned char _flag;

    void setFlag(unsigned char type, bool value) const;

    static_assert(sizeof(_flag) * 8 >= 4 );

    virtual void reset_flag();

    static std::shared_ptr<Stroke> load_ver_1(WZipReaderSingle &reader, int *ok);
    static std::shared_ptr<Stroke> load_ver_2(WZipReaderSingle &reader, int *ok);
public:    
    virtual ~Stroke() = default;

    enum type_stroke_private: int{
            COMPLEX_NORMAL = 0,
            COMPLEX_CIRCLE = 1,
            COMPLEX_RECT = 2,
            COMPLEX_LINE = 3
    };

    virtual void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const = 0;
    virtual int is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const = 0;
    virtual bool is_inside(const RectF &rect, double precision) const = 0;

    colore_s getColor(double division = 1.) const;

#   define stroke_append_default (-1.)
    virtual void append(const Point &point, pressure_t pressure) = 0;

    void setMetadata(int posizione_audio, const colore_s &color);
    void setMetadata(const metadata_stroke &metadata);
    void setPositioneAudio(int m_pos_ris);
    virtual size_t createControll() const;

    int getPosizioneAudio() const;
    virtual RectF getBiggerPointInStroke() const;
    virtual bool isInside(const RectF &rect) const = 0;

    void clearAudio();

    const struct metadata_stroke &getMetadata() const;

    bool is_highlighter() const;
    unsigned char get_alfa() const;
    virtual size_t getSizeInMemory() const = 0;

    virtual size_t getSizeInFile() const;
    virtual void decreasePrecision() = 0;
    void setAlfaColor(unsigned char alfa);

    /** instance of *this == StrokeNormal ==> @return == NULL*/
    virtual std::shared_ptr<Stroke> makeNormal() const = 0;

    /**
     * instanceof(*this) == StrokeNorml ? @result == length() : 0
    */
    virtual int how_much_decrese() const = 0;

    void setColor(const colore_s &color);

    virtual std::shared_ptr<Stroke> clone() const = 0;

    virtual bool isEmpty() const = 0;

    virtual void scale(const PointF &offset) = 0;

    virtual void adjust(double zoom) = 0;

    friend class Page;
    friend class xmlstruct;
    friend class stroke_drawer;
    friend class page_file;

    virtual bool operator==(const Stroke &other) const;
    virtual bool operator!=(const Stroke &other) const;

#ifdef DEBUGINFO
    friend class page_file;
#endif // DEBUGINFO

    static std::shared_ptr<Stroke> load(WZipReaderSingle &reader, int version_stroke, int *ok);
    virtual int save(WZipWriterSingle &file) const;

    /** all stroke derivated class needs to implements this method to recognize yourself */
    virtual int type() const = 0;

protected:
    Stroke &operator=(const Stroke &other);

    Stroke();
    void clone(Stroke &out) const;
    explicit Stroke(const metadata_stroke& met);
    virtual void modify() const;
    void setBiggerData(const RectF &newRect) const;
    bool needToUpdateBiggerData() const;

    virtual void preappend(int l) = 0;

    friend class StrokePre;
};

void set_press( WPen &pen, pressure_t press, double prop,
                cbool is_rubber, const colore_s &color);

inline Stroke &Stroke::operator=(const Stroke &other)
{
    W_ASSERT(this->type() == other.type());
    if (un(this == &other)) {
        return *this;
    }

    this->_biggerData = other._biggerData;
    this->_metadata = other._metadata;
    this->_flag = other._flag;
    return *this;
}

inline void Stroke::setBiggerData(const RectF &newRect) const
{
    this->_biggerData = newRect;
    setFlag(UPDATE_BIGGER_DATA, false);
}

inline void Stroke::modify() const
{
    _flag = UPDATE_BIGGER_DATA;

    W_ASSERT(this->needToUpdateBiggerData());
}

inline bool Stroke::needToUpdateBiggerData() const
{
    return _flag & UPDATE_BIGGER_DATA;
}

inline void Stroke::setFlag(unsigned char type, bool value) const
{
    if (value) {
        _flag |= type;
    } else {
        _flag &= ~type;
    }
}

inline void Stroke::setPositioneAudio(int m_pos_ris)
{
    W_ASSERT(m_pos_ris == -1 or m_pos_ris >= 0);
    this->_metadata.posizione_audio = m_pos_ris;
}

inline int Stroke::getPosizioneAudio() const
{
    return this->_metadata.posizione_audio;
}

inline const metadata_stroke &Stroke::getMetadata() const
{
    return this->_metadata;
}

inline bool Stroke::is_highlighter() const
{
    return _metadata.color.getAlfa() < 255;
}

inline unsigned char Stroke::get_alfa() const
{
    return this->_metadata.color.getAlfa();
}

inline void Stroke::clone(Stroke &out) const
{
    out._metadata = this->_metadata;
    out._biggerData = this->_biggerData;
    out._flag = this->_flag;
}

inline bool metadata_stroke::operator!=(const metadata_stroke &other) const
{
    return !(*this == other);
}

inline bool metadata_stroke::operator==(const metadata_stroke &other) const
{
    return  this->posizione_audio == other.posizione_audio and
            this->color == other.color;
}
