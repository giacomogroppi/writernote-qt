#ifndef SQUARE_H
#define SQUARE_H
#include <QPainter>
#include <QPointF>

#include "../../currenttitle/document.h"
#include "../property/property_control.h"
#include "../copy_cut/copy_cut_selection.h"

class square: public QObject
{
    Q_OBJECT
public:
    bool somethingInBox() const;

    void needReload(QPainter &painter, const QWidget *pixmap);

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

    void updatePoint(const QPointF &puntofine);
    bool find(Document *data);

    bool isinside(const QPointF &);
    void move(const QPointF &);

    /* definizione per i punti di spostamento */
    PointSettable lastpoint;

private:
    /*
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool __need_reload = false;
    void findObjectToDraw();

    PointSettable pointinit;
    PointSettable pointfine;

    QList<int> m_id; /* point */
    QList<int> m_index_img; /* image */

    QPen penna;
    class property_control *m_property;

    /* if true: it means that the user has not previously selected comething */
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

#endif // SQUARE_H
