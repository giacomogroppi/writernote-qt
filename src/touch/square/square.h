#ifndef SQUARE_H
#define SQUARE_H

#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QList>

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

    void reset(bool paste);

    int calculate_flags() const;

    void updatePoint(const QPointF &puntofine);
    bool find();

    bool isinside(const QPointF &);
    void move(const QPointF &);

    /* definizione per i punti di spostamento */
    PointSettable lastpoint;

    void changeInstrument(cbool paste);

    void isMoving() { /*m_property->Hide();*/ };
    void endMoving(const QWidget *pixmap);

    void translate(const QPointF &offset);


private:
    void findObjectToDraw(const QList<QVector<int> > index);
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
        this->m_property->Hide();
    }
}

Q_ALWAYS_INLINE void square::changeInstrument(cbool paste)
{
    this->reset(paste);
    this->m_property->Hide();
}

inline void square::translate(const QPointF &offset)
{
    this->pointinit += offset;
    this->pointfine += offset;

    this->m_property->Hide();
}

#endif // SQUARE_H
