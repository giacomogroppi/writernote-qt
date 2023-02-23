#pragma once

#include <QPainter>
#include <QPointF>
#include <QVector>
#include <QList>

#include "currenttitle/document.h"
#include "touch/property/property_control.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/square/SquareMethod.h"

constexpr bool debugSquare = false;

class square:   public QObject,
                public SquareMethod
{
    Q_OBJECT
public:
    [[nodiscard]] bool somethingInBox() const;

    void needReload(QPainter &painter);

    void adjustPoint();

    explicit square(QObject *parent, class property_control *property);
    ~square() override;

    void reset();

    [[nodiscard]] int calculate_flags() const;

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

#ifdef DEBUGINFO
    PointSettable & get_first_point()
    {
        return this->_pointinit;
    }

    PointSettable & get_last_point()
    {
        return this->_pointfine;
    }

#endif // DEBUGINFO

private:
    void findObjectToDrawImg();
    void findObjectToDraw(const QList<QVector<int> > &index);
    void initImg();
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool _need_reload = false;

    PointSettable _pointinit;
    PointSettable _pointfine;
    void mergeImg(const WImage &from, WImage &to, int page);
    void moveObjectIntoPrivate(QList<QVector<int> > &index);

    WImage _img;
    QList<QList<std::shared_ptr<Stroke>>> _stroke;

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
