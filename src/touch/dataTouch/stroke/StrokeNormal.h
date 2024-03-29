#pragma once

#include "Stroke.h"
#include "touch/dataTouch/page/Page.h"
#include "core/WListFast.h"
#include "core/WPainter/WPainterSafe.h"

class StrokeNormal final: public Stroke{
private:
    void rep () const;
    int save(WritableAbstract &file) const final;
    /**
     * you need to set len_point only if you are loading a version 1 file
     * */
    int load(WZipReaderSingle &reader, int version, int len_point = -1);

    WListFast<PointF> _point;
    WVector<pressure_t> _pressure;
    bool isInsideBiggerData(const RectF &rect) const;
    int removeAt(int i);

    int load_ver_1(WZipReaderSingle &reader, int len_stroke);
    int load_ver_2(WZipReaderSingle &reader);

    /**
     * \tparam T Iterator for points
     * \tparam Z Iterator for pressure
    */
    template <class T = WListFast<PointF>::const_iterator, class Z = WVector<pressure_t>::const_iterator>
    class drawData {
    public:
        explicit drawData (T begin_point, T end_point, Z begin_press, bool press_null, int index_start)
                        : begin_point(begin_point)
                        , end_point(end_point)
                        , begin_press(begin_press)
                        , press_null(press_null)
                        , index_start(index_start) {}
    protected:
        T begin_point;
        const T end_point;
        Z begin_press;
        bool press_null;
        
        /**
         * TODO: translate
         * indica l'indice della lista del primo punto
         * questo serve per capire se ci sono punti precedenti
         * a quello che l'iterator punta.
         **/
        int index_start;
        
        friend class StrokeNormal;
    };

    template <class T, class Z>
    static void draw(WPainter &painter, cbool is_rubber, cint page, WPen &pen, cdouble prop, const WColor &color, drawData<T, Z> data);

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
    StrokeNormal(StrokeNormal &&other) noexcept;
    ~StrokeNormal() final;

    void draw(WPainter &painter, bool is_rubber, int page, double prop, const WColor& color) const override;
    void draw(WPainter &painter, bool is_rubber, int page, cdouble prop) const override;

    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const final;
    bool is_inside(const RectF &rect, double precision) const final;

#   define stroke_append_default (-1.)
    void append(const PointF &point, pressure_t pressure) final;
    size_t createControl() const final;

    RectF getBiggerPointInStroke() const final;
    bool isInside(const RectF &rect) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;
    void decreasePrecision() final;

    void append (WListFast<PointF> &&points, WVector<pressure_t> &&pressures) final;
    
    int how_much_decrese() const final;

    /**
     * @requires size() > to
     * @ensures old(length()) == size() - (indexTo - indexFrom + 1) &&
     *          forall(int i; 0 <= i < from; old(point.at(i)) == point.at(i)) &&
     *          forall(int i; indexFrom <= i <= size();
     *              old(point.at(indexTo - indexFrom + 1)) == point.at(i))
    */
    void removeAt(int indexFrom, int indexTo);

    /**
     * This function will remove the point belonging [index, size())
     * */
    std::shared_ptr<StrokeNormal> split(int index);

    std::unique_ptr<Stroke> clone() const final;

    void preappend(int i) final;

    bool isEmpty() const final;
    void adjust(double zoom) final;
    void scale(const PointF &offset) final;

    void force_pressure(pressure_t press);

    UniquePtr<Stroke> makeNormal() const final;

    auto operator=(const StrokeNormal &other) -> StrokeNormal&;
    auto operator==(const Stroke &other) const -> bool final;
    auto operator!=(const Stroke &other) const -> bool final;

    template<class T>
    static auto getBiggerPointInStroke(T begin, T end) -> RectF;

    auto type() const -> int final;

    /**
     * This method assume that the stroke to be load is StrokeNormal and
     * it don't load/call method to load internal data of Stroke
     */
    static
    auto loadPtr (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<Error, StrokeNormal*>;

protected:
    int length () const { return _point.size(); }
    auto getPressure() const {
        W_ASSERT(this->_pressure.at(0) >= 0.);
        return this->_pressure.at(0);
    }

#ifdef DEBUGINFO
    friend class page_file;
#endif // DEBUGINFO

    friend void actionRubberSinglePartial(DataPrivateMuThread &data);
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
    if (needToUpdatePressure())
        this->updateFlagPressure();

    rep();

    return isPressureVal();
}

inline void StrokeNormal::updateFlagPressure() const
{
    pressure_t press;
    auto &_press = (WVector<pressure_t> &) _pressure;

    this->_flag &= flag_state::UPDATE_PRESSURE;

    const auto len = this->length();

    if (len < 2 and _press.size() == 1) {
        // lo stroke era _press const ma gli abbiamo
        // tolto troppi punti.
        const auto press = _press.at(0);

        for (int i = 1; i < len; i++) {
            _press.append(press);
        }

        W_ASSERT(_press.size() == _point.size());
    }

    if (len > 1 and _press.size() == 1) {
        return;
    }

    W_ASSERT(_pressure.size() == _point.size());

    if (len < 3) {
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
        const WColor &color,
        drawData<T, Z> data)
{
    constexpr bool not_used debug_draw_stroke = false;
    auto &painterPublic = _painter;

    W_ASSERT(page >= 0);
    W_ASSERT(painterPublic.isActive());

    WPixmap img;
    WPainterUnsafe _painterPrivate;
    WPainter *painter;
    PointF lastPoint;
    const bool isHigh = pen.color().getAlfa() < 255;
    const double prop = _prop == PROP_RESOLUTION ? _prop : 1.;
    const bool isPrivatePainter = isHigh;

    if (isPrivatePainter) {
        img = WPixmap(1, true);
        img.fill(WColor(color_transparent));
        _painterPrivate.begin(&img);
        _painterPrivate.setAntialiasing();
        painter = &_painterPrivate;
    } else {
        painter = &painterPublic;
    }

    lastPoint = Page::at_translation(*data.begin_point, page) * prop;

    pen.setColor(color);

    for (data.begin_point ++; data.begin_point != data.end_point; data.begin_point ++) {
        const PointF pointDraw = Page::at_translation(*data.begin_point, page) * prop;
        const pressure_t pressure = *(data.begin_press);

        if (!data.press_null){
            data.begin_press ++;
        }

        pen.setPressure(pressure * _prop);

        painter->setPen(pen);

        if (is_rubber) {
            pen.setWidthF(pen.widthF() * deltaColorNull);
        } else if (isHigh) {
            const auto curr = painter->compositionMode();
            painter->setCompositionMode(WPainter::CompositionMode_Clear);
            painter->drawPoint(lastPoint);
            painter->setCompositionMode(curr);
        }
        
        if (data.index_start == 0 || data.index_start == 1 or true) {
            painter->drawLine(lastPoint, pointDraw);
        } else {
            // do the trick [for now disable]
            const auto previousPoint2 = Page::at_translation(*(data.begin_point - 2), page) * prop;
            const auto previousPoint1 = Page::at_translation(*(data.begin_point - 1), page) * prop;
            
            const auto mid1 = PointF::mid(previousPoint1, previousPoint2);
            const auto mid2 = PointF::mid(pointDraw, previousPoint1);
            
            painter->move(mid1);
            painter->addCurve(mid2, previousPoint1);
            painter->closePath();
        }
        
        data.index_start ++;
        
        lastPoint = pointDraw;
    }

    if (isPrivatePainter) {
        W_ASSERT(isHigh);
        W_ASSERT(painterPublic.compositionMode() == WPainter::CompositionMode_SourceOver);

        WDebug(debug_draw_stroke, "Paint high" << painterPublic.compositionMode());

        painter->end();

        const auto r = img.rect();
        painterPublic.drawPixmap(
                WRect(
                        WPoint{r.topLeft().x(), r.topLeft().y()},
                        WPoint{r.bottomRight().x(), r.bottomRight().y()}
            ),
                img);
    } else {
        WDebug(debug_draw_stroke, "Paint not high");
    }
}

template<class T>
inline auto StrokeNormal::getBiggerPointInStroke(T begin, T end) -> RectF
{
    RectF biggerData;

    if (begin == end) {
        WWarning("Warning: Stroke empty");
        return {0, 0, 0, 0};
    }

    PointF topLeft      = *begin;
    PointF bottomRight  = *begin;

    for (; begin != end; begin ++) {
        const PointF &point = *begin;

        topLeft.setX(std::min(topLeft.x(), point.x()));
        topLeft.setY(std::min(topLeft.y(), point.y()));

        bottomRight.setX(std::max(bottomRight.x(), point.x()));
        bottomRight.setY(std::max(bottomRight.y(), point.y()));
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
