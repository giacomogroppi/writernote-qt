#pragma once

#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QList>

#include "currenttitle/document.h"
#include "touch/property/property_control.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/common_script.h"
#include "touch/multi_thread_data.h"

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
    PointSettable _lastpoint;

    void changeInstrument();

    void hideProperty();

    void isMoving();
    void endMoving(const QWidget *pixmap);

    void translate(const QPointF &offset);

    PointSettable & get_first_point()
    {
#ifdef DEBUGINFO
        return _pointinit;
#else
        qDebug() << "Call to PointSettable & square::get_first_point in release mode";
        std::abort();
#endif
    };

    PointSettable & get_last_point()
    {
#ifdef DEBUGINFO
        return _pointfine;
#else
        qDebug() << "Call to PointSettable & square::get_last_point in release mode";
        std::abort();
#endif
    }

private:
    void findObjectToDrawImg();
    void findObjectToDraw(const QList<QVector<int> > &index);
    void initImg();
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;

    PointSettable _pointinit;
    PointSettable _pointfine;
    void mergeImg(const WImage &from, WImage &to, int page);
    void moveObjectIntoPrivate(QList<QVector<int> > &index);

    WImage _img;
    QList<QList<stroke>> _stroke;

    int _base;
    QList<int> _index_img; /* image */

    QPen _penna;
    class property_control *_property;

    bool _in_box;
    copy *_copy;

    class TabletCanvas *_canvas;

    pthread_t *_thread;
    DataPrivateMuThread *_dataThread;
    int _threadCount;

    QPointF _trans_img;

private slots:
    void actionProperty(property_control::ActionProperty action);
};

force_inline bool square::somethingInBox() const
{
    return this->_in_box;
}

inline int square::calculate_flags() const
{
    int flag = 0;

    if(this->somethingInBox()){
        flag = PROPERTY_SHOW_DELETE | PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT;
    }else{
        if(!_copy->isEmpty()){
            flag = PROPERTY_SHOW_PASTE;
        }
    }

    W_ASSERT(flag >= 0);

    return flag;
}

force_inline void square::changeInstrument()
{
    WDebug(debugSquare, "square::changeInstrument");
    this->reset();
    this->_property->Hide();
}

inline void square::hideProperty()
{
    this->_property->Hide();
}

inline void square::translate(const QPointF &offset)
{
    Q_UNUSED(offset);
    WDebug(debugSquare, "square::translate");
    if(likely(!somethingInBox()))
        return;

    //this->_property->Hide();
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
    return datastruct_isinside(_pointinit, _pointfine, point);
}
