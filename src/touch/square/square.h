#ifndef SQUARE_H
#define SQUARE_H

#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QList>

#include "currenttitle/document.h"
#include "touch/property/property_control.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/common_script.h"

constexpr bool debugSquare = false;

class square: public QObject
{
    Q_OBJECT
public:
    bool somethingInBox() const;

    void needReload(QPainter &painter);

    void adjustPoint();

    explicit square(QObject *parent, class property_control *property);
    ~square();

    void reset();

    int calculate_flags() const;

    void initPoint(const QPointF &point);
    void updatePoint(const QPointF &puntofine);
    bool find();

    bool isinside(const QPointF &);

    void move(const QPointF & point);
    void initPointMove(const QPointF & point);

    /* definizione per i punti di spostamento */
    PointSettable lastpoint;

    void changeInstrument();

    void isMoving();
    void endMoving(const QWidget *pixmap);

    void translate(const QPointF &offset);

#ifdef DEBUGINFO
    PointSettable & get_first_point() {return pointinit;};
#endif

private:
    void findObjectToDraw(const QList<QVector<int> > &index);
    void initImg();
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;

    PointSettable pointinit;
    PointSettable pointfine;
    void mergeImg(const QImage &from, QImage &to, int page);
    void moveObjectIntoPrivate(QList<QVector<int> > &index);

    QPointF trans_img;
    QImage img;
    QList<QList<stroke>> m_stroke;

    int base;
    QList<int> m_index_img; /* image */

    QPen penna;
    class property_control *m_property;

    bool in_box;
    copy *m_copy;

    class TabletCanvas *canvas;

private slots:
    void actionProperty(property_control::ActionProperty action);
};

force_inline bool square::somethingInBox() const
{
    return this->in_box;
}

inline int square::calculate_flags() const
{
    int flag = 0;

    if(this->somethingInBox()){
        flag = PROPERTY_SHOW_DELETE | PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT;
    }else{
        if(!m_copy->isEmpty()){
            flag = PROPERTY_SHOW_PASTE;
        }
    }

    W_ASSERT(flag >= 0);

    return flag;
}

inline void square::initPoint(const QPointF &point)
{
    WDebug(debugSquare, "square::initPoint");
    pointinit.point = point;
    pointinit.set = true;

    /* we don't need yet to draw somethings */
    __need_reload = false;
    in_box = false;

    this->m_property->Hide();
}

force_inline void square::updatePoint(const QPointF &puntofine)
{
    WDebug(debugSquare, "square::updatePoint");
    pointfine.point = puntofine;
    __need_reload = true;
}

force_inline void square::changeInstrument()
{
    WDebug(debugSquare, "square::changeInstrument");
    this->reset();
    this->m_property->Hide();
}

inline void square::translate(const QPointF &offset)
{
    WDebug(debugSquare, "square::translate");
    if(likely(!somethingInBox()))
        return;

    this->pointinit += offset;
    this->pointfine += offset;

    this->trans_img += offset;

    this->m_property->Hide();
}

force_inline void square::isMoving()
{

}

/* la funzione resistuisce
 * vero se è intero il punto è interno
*/
force_inline bool square::isinside(const QPointF &point)
{
    WDebug(debugSquare, "square::isinside");
    return datastruct::isinside(pointinit.point, pointfine.point, point);
}

#endif // SQUARE_H
