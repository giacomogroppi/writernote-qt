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
#include "touch/multi_thread_data.h"

constexpr bool debugSquare = true;

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
    void updatePoint(const QPointF &puntofine, const double zoom);
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
    PointSettable & get_first_point()   {return _pointinit;};
    PointSettable & get_last_point()    {return _pointfine;}
#endif

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
    void mergeImg(const QImage &from, QImage &to, int page);
    void moveObjectIntoPrivate(QList<QVector<int> > &index);

    QImage _img;
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

force_inline void square::updatePoint(const QPointF &puntofine, cdouble zoom)
{
    WDebug(debugSquare, "square::updatePoint");

    _pointfine.point = puntofine / zoom;
    _pointfine.set = true;

    __need_reload = true;
}

force_inline void square::changeInstrument()
{
    WDebug(debugSquare, "square::changeInstrument");
    this->reset();
    this->_property->Hide();
}

inline void square::translate(const QPointF &offset)
{
    WDebug(debugSquare, "square::translate");
    if(likely(!somethingInBox()))
        return;

    this->_pointinit += offset;
    this->_pointfine += offset;

    this->_property->Hide();
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
    return datastruct::isinside(_pointinit.point, _pointfine.point, point);
}

#endif // SQUARE_H
