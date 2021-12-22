#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

#include "currenttitle/document.h"
#include "touch/property/property_control.h"
#include "touch/copy_cut/copy_cut_selection.h"

class square: public QObject
{
    Q_OBJECT
public:
    bool somethingInBox() const;

    void needReload(QPainter &painter);

    void adjustPoint();

    explicit square(QObject *parent, class property_control *property);
    ~square();

    inline void reset(){
        pointinit.set = lastpoint.set = pointfine.set = false;

        in_box = false;

        __need_reload = false;
        m_id.clear();
        m_index_img.clear();
    }

    int calculate_flags() const;

    void updatePoint(const QPointF &puntofine);
    bool find(Document *data);

    bool isinside(const QPointF &);
    void move(const QPointF &);

    /* definizione per i punti di spostamento */
    PointSettable lastpoint;

    void changeInstrument(){
        this->reset();
        this->m_property->Hide();
    };

    void isMoving() { /*m_property->Hide();*/ };
    void endMoving(const QWidget *pixmap);

    void translate(const QPointF &offset);

private:
    void findObjectToDraw();

private:
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;

    PointSettable pointinit;
    PointSettable pointfine;

    QList<int> m_id; /* point */
    QList<int> m_index_img; /* image */

    QPen penna;
    class property_control *m_property;

    bool in_box: 1;
    copy *m_copy;

    class TabletCanvas *canvas;

private slots:
    void actionProperty(property_control::ActionProperty action);
};

inline bool square::somethingInBox() const
{
    return this->in_box;
}

inline int square::calculate_flags() const
{
    int flag = 0;

    if(this->somethingInBox()){
        flag = PROPERTY_SHOW_DELETE | PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT;
    }else{
        if(!m_copy->isEmpty())
            flag = PROPERTY_SHOW_PASTE;

    }

    Q_ASSERT(flag >= 0);

    return flag;
}

Q_ALWAYS_INLINE void square::updatePoint(const QPointF &puntofine)
{
    if(pointinit.set){
        pointfine.point = puntofine;
        __need_reload = true;
    }
    else{
        pointinit.point = puntofine;
        pointinit.set = true;

        /* we don't need yet to draw somethings */
        __need_reload = false;
        in_box = false;
    }
}

inline void square::translate(const QPointF &offset)
{
    this->pointinit += offset;
    this->pointfine += offset;

    this->m_property->Hide();
}

Q_ALWAYS_INLINE void square::needReload(QPainter &painter)
{
    if(__need_reload){
        painter.setPen(this->penna);
        painter.drawRect(QRectF(pointinit.point, pointfine.point));
    }
}

#endif // SQUARE_H
