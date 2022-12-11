#pragma once

#include "Stroke.h"
#include <QList>
#include <QPainter>
#include "touch/dataTouch/page/Page.h"

class StrokeNormal final: public Stroke{
private:
    int save(WZipWriterSingle &file) const final;
    /**
     * you need to set len_point only if you are loading a version 1 file
     * */
    int load(WZipReaderSingle &reader, int version, int len_point = -1);

    QList<point_s> _point;
    QList<pressure_t> _pressure;
    bool isInsideBiggerData(const QRect &rect) const;
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
    static void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop, const QColor &color, drawData<T, Z> data);

    enum flag_state: unsigned char {
        UPDATE_PRESSURE = BIT(2)
    };

    mutable typeof(flag_state) _flag;

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

    void draw(QPainter &painter, cbool is_rubber, cint page, QPen &pen, cdouble prop) const override;
    int is_inside(const WLine &line, int from, int precision, cbool needToDeletePoint) const final;
    bool is_inside(const QRectF &rect, double precision) const final;

#   define stroke_append_default (-1.)
    void append(const point_s &point, pressure_t pressure) final;
    size_t createControll() const final;

    QRect getBiggerPointInStroke() const final;
    bool isInside(const QRectF &rect) const final;

    size_t getSizeInMemory() const final;
    size_t getSizeInFile() const final;
    void decreasePrecision() final;

    int how_much_decrese() const final;

    /**
     * @requires length() > to
     * @ensures \old(length()) == length() - (indexTo - indexFrom + 1) &&
     *          \forall(int i; 0 <= i < from; \old(point.at(i)) == point.at(i)) &&
     *          \forall(int i; indexFrom <= i <= length();
     *              \old(point.at(indexTo - indexFrom + 1)) == point.at(i))
    */
    void removeAt(int indexFrom, int indexTo);

    /**
     * This function will remove the point belonging [index, length())
     * */
    std::shared_ptr<StrokeNormal> split(int index);

    std::shared_ptr<Stroke> clone() const final;

    void preappend(int i) final;

    bool isEmpty() const final;
    void adjust(double zoom) final;
    void scale(const QPointF &offset) final;

    void force_pressure(pressure_t press);

    std::shared_ptr<Stroke> makeNormal() const final;

    StrokeNormal& operator=(const StrokeNormal &other);
    bool operator==(const Stroke &other) const final;
    bool operator!=(const Stroke &other) const final;

    template<class T>
    static inline QRect getBiggerPointInStroke(T begin, T end);

    int type() const final;

protected:
    auto length () const { return _point.length(); }
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
    return _pressure.length() == 1 && _point.length() > 1;
}

inline bool StrokeNormal::needToUpdatePressure() const
{
    return !!(this->_flag & flag_state::UPDATE_PRESSURE);
}

inline bool StrokeNormal::constantPressure() const
{
    if(un(needToUpdatePressure()))
        this->updateFlagPressure();

    return isPressureVal();
}

inline void StrokeNormal::updateFlagPressure() const
{
    int len, i;
    pressure_t press;
    auto &_press = (QList<pressure_t> &)_pressure;

    ((unsigned char &)(this->_flag)) &= flag_state::UPDATE_PRESSURE;

    len = this->length();

    if(un(len < 2 && _press.length() == 1)){
        // lo stroke era _press const ma gli abbiamo
        // tolto troppi punti, quindi non possiamo più disegnarlo
        // con il QPainterPath
        int i;
        const auto press = _press.at(0);

        for(i = 1; i < len; i++){
            _press.append(press);
        }

        W_ASSERT(_press.length() == _point.length());
    }

    if(un(len > 1 && _press.length() == 1)){
        return;
    }

    W_ASSERT(_pressure.length() == _point.length());

    if(un(len < 3)){
        /**
         * if we have less than 3 points we
         * cannot create a qpainterpath, so
         * we have to draw the stroke point
         * by point.
        */
        return;
    }

    press = _pressure.at(0);

    for(i = 0; i < len; i++){
        if(_pressure.at(i) != press){
            return;
        }
    }

    _press.clear();
    _press.append(press);
}

template<class T, class Z>
force_inline void StrokeNormal::draw(
        QPainter &_painter,
        cbool is_rubber,
        cint page,
        QPen &pen,
        cdouble _prop,
        const QColor &color,
        drawData<T, Z> data)
{
    constexpr bool not_used debug_draw_stroke = false;
    auto &painterPublic = _painter;

    W_ASSERT(page >= 0);
    W_ASSERT(painterPublic.isActive());

    WImage img;
    QPainter _painterPrivate;
    QPainter *painter;
    QPointF lastPoint, pointDraw;
    cbool isHigh = pen.color().alpha() < 255;
    cdouble prop = _prop == PROP_RESOLUTION ? _prop : 1.;
    cbool isPrivatePainter = isHigh;

    if(isPrivatePainter){
        img = WImage(1);
        _painterPrivate.begin(&img);
        SetRenderPainter(_painterPrivate);
        painter = &_painterPrivate;
    }else{
        painter = &painterPublic;
    }

    lastPoint = Page::at_translation(*data.begin_point, page).toQPointF(prop);

    for(data.begin_point ++; data.begin_point != data.end_point; data.begin_point ++){
        const point_s point = Page::at_translation(*data.begin_point, page);
        const pressure_t pressure = *data.begin_press;

        if(!data.press_null){
            data.begin_press ++;
        }

        pointDraw = point.toQPointF(prop);

        set_press(pen, pressure, _prop, is_rubber, color);
        painter->setPen(pen);

        if (un(is_rubber)) {
            pen.setWidthF(pen.widthF() * deltaColorNull);
        }
        else if(un(isHigh)){
            const QPainter::CompositionMode curr = painter->compositionMode();
            painter->setCompositionMode(QPainter::CompositionMode_Clear);
            painter->drawPoint(lastPoint);
            painter->setCompositionMode(curr);
        }

        painter->drawLine(lastPoint, pointDraw);

        lastPoint = pointDraw;
    }

    if(likely(isPrivatePainter)){
        W_ASSERT(isHigh);
        W_ASSERT(painterPublic.compositionMode() == QPainter::CompositionMode_SourceOver);

        WDebug(debug_draw_stroke, "Paint high" << painterPublic.compositionMode());

        painter->end();

        painterPublic.drawImage(img.rect(), img);
    } else {
        WDebug(debug_draw_stroke, "Paint not high");
    }
}

template<class T>
inline QRect StrokeNormal::getBiggerPointInStroke(T begin, T end)
{
    QRect biggerData;

    if(un(begin == end)){
        WWarning("Warning: Stroke empty");
        return {0, 0, 0, 0};
    }

    QPoint topLeft = begin->toQPointF(1.).toPoint();
    QPoint bottomRight = end->toQPointF(1.).toPoint();

    for(; begin != end; begin ++){
        const point_s &point = *begin;

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

    biggerData = QRect(topLeft, bottomRight);

    return biggerData;
}

inline void StrokeNormal::reset_flag()
{
    this->_flag = flag_state::UPDATE_PRESSURE;
}
