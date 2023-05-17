#pragma once

#include "Stroke.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WListFast.h"

class StrokeNormal final: public Stroke{
private:
    void rep () const;
    int save(WZipWriterSingle &file) const final;
    /**
     * you need to set len_point only if you are loading a version 1 file
     * */
    int load(WZipReaderSingle &reader, int version, int len_point = -1);

    WListFast<PointF> _point;
    WListFast<pressure_t> _pressure;
    bool isInsideBiggerData(const RectF &rect) const;
    int removeAt(int i);

    int load_ver_1(WZipReaderSingle &reader, int len_stroke);
    int load_ver_2(WZipReaderSingle &reader);

    /**
     * T and Z must be iterator
    */

    template <class T, class Z>
    struct drawData{
        T begin_point, end_point;
        Z begin_press, end_press;
        bool press_null;
    };

    template <class T, class Z>
    static void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop, const colore_s &color, drawData<T, Z> data);

    enum flag_state: unsigned char {
        UPDATE_PRESSURE = BIT(2)
    };

    mutable unsigned char _flag;

    void modify() const final;
    void reset_flag() final;
    bool constantPressure() const;
    void updateFlagPressure() const;
    bool isPressureVal() const;
    bool needToUpdatePressure() const;

public:
    StrokeNormal(const StrokeNormal &ref);
    StrokeNormal();
    ~StrokeNormal() final;

    void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop) const override;
    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const final;
    bool is_inside(const RectF &rect, double precision) const final;

#   define stroke_append_default (-1.)
    void append(const PointF &point, pressure_t pressure) final;
    size_t createControll() const final;

    RectF getBiggerPointInStroke() const final;
    bool isInside(const RectF &rect) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;
    void decreasePrecision() final;

    int how_much_decrese() const final;

    /**
     * @requires size() > to
     * @ensures \old(length()) == size() - (indexTo - indexFrom + 1) &&
     *          \forall(int i; 0 <= i < from; \old(point.at(i)) == point.at(i)) &&
     *          \forall(int i; indexFrom <= i <= size();
     *              \old(point.at(indexTo - indexFrom + 1)) == point.at(i))
    */
    void removeAt(int indexFrom, int indexTo);

    /**
     * This function will remove the point belonging [index, size())
     * */
    std::shared_ptr<StrokeNormal> split(int index);

    std::shared_ptr<Stroke> clone() const final;

    void preappend(int i) final;

    bool isEmpty() const final;
    void adjust(double zoom) final;
    void scale(const PointF &offset) final;

    void force_pressure(pressure_t press);

    std::shared_ptr<Stroke> makeNormal() const final;

    StrokeNormal& operator=(const StrokeNormal &other);
    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    template<class T>
    static inline RectF getBiggerPointInStroke(T begin, T end);

    int type() const final;

protected:
    int length () const { return _point.size(); }
    auto getPressure() const {
        W_ASSERT(this->_pressure.at(0) >= 0.);
        return this->_pressure.at(0);
    }

#ifdef DEBUGINFO
    friend class page_file;
#endif // DEBUGINFO

    friend void actionRubberSinglePartial(DataPrivateMuThread *data);
    friend class StrokeCircleGenerator;
    friend class StrokeLineGenerator;
    friend class Stroke;
    friend class StrokePre;
    friend class StrokeLine;
    friend class StrokeNormalFileLoad;
    friend class Page;
    friend class StrokeForPage;
    friend class stroke_drawer;
    friend class stroke_file;
    friend class page_file;
    friend void stroke_complex_adjust(Stroke *stroke, cdouble zoom);
};

inline bool StrokeNormal::isPressureVal() const
{
    rep();
    return _pressure.size() == 1 and _point.size() > 1;
}

inline bool StrokeNormal::needToUpdatePressure() const
{
    rep();
    return !!(this->_flag & flag_state::UPDATE_PRESSURE);
}

inline bool StrokeNormal::constantPressure() const
{
    if (un(needToUpdatePressure()))
        this->updateFlagPressure();

    rep();

    return isPressureVal();
}

inline void StrokeNormal::updateFlagPressure() const
{
    pressure_t press;
    auto &_press = (WListFast<pressure_t> &) _pressure;

    this->_flag &= flag_state::UPDATE_PRESSURE;

    const auto len = this->length();

    if (len < 2 and _press.size() == 1) {
        // lo stroke era _press const ma gli abbiamo
        // tolto troppi punti.
        const auto press = _press.at(0);

        for (int i = 1; i < len; i++) {
            _press.append(press);
        }

        W_ASSERT(_press.length() == _point.length());
    }

    if (len > 1 and _press.size() == 1) {
        return;
    }

    W_ASSERT(_pressure.length() == _point.length());

    if (un(len < 3)) {
        /**
         * if we have less than 3 points we
         * cannot create a WPainterpath, so
         * we have to draw the stroke point
         * by point.
        */
        return;
    }

    press = _pressure.at(0);

    for (int i = 0; i < len; i++) {
        if (_pressure.at(i) != press) {
            return;
        }
    }

    _press.clear();
    _press.append(press);

    rep();
}

template<class T, class Z>
force_inline void StrokeNormal::draw(
        WPainter &_painter,
        cbool is_rubber,
        cint page,
        WPen &pen,
        cdouble _prop,
        const colore_s &color,
        drawData<T, Z> data)
{
    constexpr bool not_used debug_draw_stroke = false;
    auto &painterPublic = _painter;

    W_ASSERT(page >= 0);
    W_ASSERT(painterPublic.isActive());

    WPixmap img;
    WPainter _painterPrivate;
    WPainter *painter;
    PointF lastPoint, pointDraw;
    cbool isHigh = pen.color().getAlfa() < 255;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;
    cbool isPrivatePainter = isHigh;

    if (isPrivatePainter) {
        img = WPixmap(1, true);
        img.fill(color_transparent);
        _painterPrivate.begin(&img);
        _painterPrivate.setAntialeasing();
        painter = &_painterPrivate;
    } else {
        painter = &painterPublic;
    }

    lastPoint = Page::at_translation(*data.begin_point, page) * prop;

    for (data.begin_point ++; data.begin_point != data.end_point; data.begin_point ++) {
        const PointF point = Page::at_translation(*data.begin_point, page);
        const pressure_t pressure = *data.begin_press;

        if(!data.press_null){
            data.begin_press ++;
        }

        pointDraw = point * prop;

        set_press(pen, pressure, _prop, is_rubber, color);
        painter->setPen(pen);

        if (un(is_rubber)) {
            pen.setWidthF(pen.widthF() * deltaColorNull);
        }
        else if (un(isHigh)) {
            const auto curr = painter->compositionMode();
            painter->setCompositionMode(WPainter::CompositionMode_Clear);
            painter->drawPoint(lastPoint);
            painter->setCompositionMode(curr);
        }

        painter->drawLine(lastPoint, pointDraw);

        lastPoint = pointDraw;
    }

    if (isPrivatePainter) {
        W_ASSERT(isHigh);
        W_ASSERT(painterPublic.compositionMode() == WPainter::CompositionMode_SourceOver);

        WDebug(debug_draw_stroke, "Paint high" << painterPublic.compositionMode());

        painter->end();

        painterPublic.drawPixmap(img.rect(), img);
    } else {
        WDebug(debug_draw_stroke, "Paint not high");
    }
}

template<class T>
inline RectF StrokeNormal::getBiggerPointInStroke(T begin, T end)
{
    RectF biggerData;

    if(un(begin == end)){
        WWarning("Warning: Stroke empty");
        return {0, 0, 0, 0};
    }

    PointF topLeft      = *begin;
    PointF bottomRight  = *begin;

    for(; begin != end; begin ++){
        const PointF &point = *begin;

        if(topLeft.x() > point.x())
            topLeft.setX(static_cast<int>(
                                 point.x()
                         ));

        if(topLeft.y() > point.y())
            topLeft.setY(static_cast<int>(
                                 point.y()
                         ));

        if(bottomRight.x() < point.x())
            bottomRight.setX(static_cast<int>(
                                     point.x())
            );

        if(bottomRight.y() < point.y())
            bottomRight.setY(static_cast<int>(
                                     point.y()
                             )
            );
    }

    W_ASSERT(topLeft.x() <= bottomRight.x());
    W_ASSERT(topLeft.y() <= bottomRight.y());

    biggerData = RectF(topLeft, bottomRight);

    return biggerData;
}

force_inline void StrokeNormal::rep() const
{
#ifdef DEBUGINFO
    W_ASSERT((this->_point.size() == this->_pressure.size()) or (_pressure.size() == 1 and _point.size() > 1));
#endif
}

inline void StrokeNormal::reset_flag()
{
    this->_flag = flag_state::UPDATE_PRESSURE;
    rep();
}
