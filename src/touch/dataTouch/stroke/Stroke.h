#pragma once

#include <memory>
#include "utils/WCommonScript.h"
#include "touch/dataTouch/Point.h"
#include "utils/common_def.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "core/WLine.h"
#include "testing/memtest.h"
#include "FileContainer/WZipWriterSingle.h"
#include "FileContainer/WZipReaderSingle.h"
#include "core/Image/WImage.h"
#include "core/WPainter/WPainter.h"
#include "core/Point.h"
#include "core/Pen/WPen.h"
#include "utils/common_error_definition.h"
#include "core/Color/WColor.h"
#include "core/WVector.h"
#include "AudioPosition.h"

#define PROP_RESOLUTION (2.)
constexpr double deltaPress = 2.;
constexpr double deltaColorNull = 1.3;

struct metadata_stroke{
    AudioPosition posizione_audio;
    class WColor color;

    metadata_stroke () = default;
    metadata_stroke (AudioPosition position_audio, const WColor &c)
        : posizione_audio(position_audio)
        , color(c)
    {

    }

    metadata_stroke(const metadata_stroke &other) noexcept = default;

    metadata_stroke (metadata_stroke && other) noexcept
        : posizione_audio(std::move(other.posizione_audio))
        , color(std::move(other.color))
    {
    }

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &reader)
        -> WPair<Error, metadata_stroke>
    {
        metadata_stroke result;

        if (versionController.getVersionMetadataStroke() != 0)
            return {Error::makeErrVersion(), result};

        {
            auto [res, data] = AudioPosition::load (versionController, reader);
            if (res)
                return {res, {}};
            result.posizione_audio = std::move(data);
        }

        {
            auto [res, color] = WColor::load (versionController, reader);
            if (res)
                return {res, {}};

            result.color = std::move (color);
        }

        return {Error::makeOk(), result};
    }

    static
    auto write (WritableAbstract &writable, const metadata_stroke &metadata) -> Error
    {
        if (auto err = AudioPosition::write(writable, metadata.posizione_audio))
            return err;
        if (auto err = WColor::write(writable, metadata.color))
            return err;

        return Error::makeOk();
    }

    auto operator!=(const metadata_stroke &other) const -> bool;
    auto operator==(const metadata_stroke &other) const -> bool;

    auto operator=(const metadata_stroke &other) noexcept -> metadata_stroke & = default;
    auto operator=(metadata_stroke &&other) noexcept -> metadata_stroke & = default;
};

class Stroke
{
private:
    struct metadata_stroke _metadata = {};
    mutable RectF _biggerData = {};

    enum flag_status : unsigned char{
        UPDATE_BIGGER_DATA = BIT(1)
    };

    using typeOfFlag = unsigned char;
    mutable typeOfFlag _flag;

    void setFlag(unsigned char type, bool value) const;

    static_assert(sizeof(_flag) * 8 >= 4 );

    virtual void reset_flag();

    static auto load_ver_1(WZipReaderSingle &reader, int *ok) -> SharedPtr<Stroke>;
    static auto load_ver_2(WZipReaderSingle &reader, int *ok) -> SharedPtr<Stroke>;
public:    
    virtual ~Stroke() = default;

    // TODO: remove this enum
    enum type_stroke_private: int {
            COMPLEX_NORMAL = 0,
            COMPLEX_CIRCLE = 1,
            COMPLEX_RECT = 2,
            COMPLEX_LINE = 3
    };

    virtual void draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor& color) const = 0;

    /**
     * \param painter The painter to use for draw. It needs to be active and it's guarantee to be active at
     *  the end of this method.
     * \param page The page at which the stroke belongs
     * */
    virtual void draw(WPainter &painter, bool is_rubber, int page, double prop) const = 0;
    
    /**
     * \return -1 in case the stroke is not touched by the line
     */
    [[deprecated]]
    virtual auto is_inside(const WLine &rect, int from, int precision, cbool needToDeletePoint) const -> int = 0;
    virtual auto is_inside(const RectF &rect, double precision) const -> bool = 0;

    auto getColor(double division = 1.) const -> WColor;

    virtual void append (const PointF &point, pressure_t pressure) = 0;
    virtual void append (WListFast<PointF> &&points, WVector<pressure_t> &&pressure) = 0;
    
    void setMetadata(AudioPosition posizione_audio, const WColor &color);
    void setMetadata(const metadata_stroke &metadata);
    void setPositionAudio(AudioPosition m_pos_ris);
    virtual auto createControl() const -> size_t;

    auto getPositionAudio() const -> AudioPosition;
    virtual auto getBiggerPointInStroke() const -> RectF;
    virtual auto isInside(const RectF &rect) const -> bool = 0;

    void clearAudio();

    auto getMetadata() const -> const struct metadata_stroke &;

    auto is_highlighter() const -> bool;
    auto getAlfa() const -> unsigned char;
    virtual auto getSizeInMemory() const -> size_t = 0;

    virtual auto getSizeInFile() const -> size_t;
    virtual void decreasePrecision() = 0;
    void setAlfaColor(unsigned char alfa);

    /** instance of *this == StrokeNormal ==> @return == NULL*/
    virtual auto makeNormal() const -> std::unique_ptr<Stroke> = 0;

    /**
     * instanceof(*this) == StrokeNorml ? @result == size() : 0
    */
    virtual auto how_much_decrese() const -> int = 0;

    void setColor(const WColor &color);

    virtual auto clone() const -> std::unique_ptr<Stroke> = 0;

    virtual auto isEmpty() const -> bool = 0;

    virtual void scale(const PointF &offset) = 0;

    virtual void adjust(double zoom) = 0;

    friend class Page;
    friend class xmlstruct;
    friend class stroke_drawer;
    friend class page_file;

    virtual auto operator==(const Stroke &other) const -> bool;
    virtual auto operator!=(const Stroke &other) const -> bool;

#ifdef DEBUGINFO
    friend class page_file;
#endif // DEBUGINFO

    // new way
    static auto loadPtr (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, Stroke*>;
    static auto write (WritableAbstract &writable, const Stroke &stroke) -> Error;

    /** all stroke derivated class needs to implements this method to recognize yourself */
    virtual int type() const = 0;

protected:
    // old way
    static auto load(WZipReaderSingle &reader, int version_stroke, int *ok) -> std::shared_ptr<Stroke>;

    /**
     * All the class that extend this method needs to call Stroke::save and than write
     * his content, because load expect this structure.
     * */
    virtual auto save(WritableAbstract &file) const -> int;

    auto operator=(const Stroke &other) -> Stroke&;

    Stroke();
    Stroke(Stroke &&other) noexcept ;

    void clone(Stroke &out) const;
    explicit Stroke(const metadata_stroke& met);
    virtual void modify() const;
    void setBiggerData(const RectF &newRect) const;
    auto needToUpdateBiggerData() const -> bool;

    virtual void preappend(int l) = 0;

    friend class StrokePre;
};

inline Stroke &Stroke::operator=(const Stroke &other)
{
    W_ASSERT(this->type() == other.type());
    if (this == &other)
        return *this;

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

inline void Stroke::setPositionAudio(AudioPosition m_pos_ris)
{
    this->_metadata.posizione_audio = m_pos_ris;
}

inline auto Stroke::getPositionAudio() const -> AudioPosition
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

inline unsigned char Stroke::getAlfa() const
{
    return this->_metadata.color.getAlfa();
}

inline void Stroke::clone(Stroke &out) const
{
    out._metadata = this->_metadata;
    out._biggerData = this->_biggerData;
    out._flag = this->_flag;
}

inline Stroke::Stroke(Stroke &&other) noexcept
    : _metadata(std::move(other._metadata))
    , _biggerData(other._biggerData)
    , _flag(other._flag)
{

}

inline auto Stroke::write(WritableAbstract &writable, const Stroke &stroke) -> Error
{
    // TODO: adjust stroke.save to return Error class
    return stroke.save(writable) == ERROR ? Error::makeCorruption() : Error::makeOk();
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
