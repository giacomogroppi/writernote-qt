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

    int calculate_flags() const;

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

inline int square::calculate_flags() const
{
    int flag = 0;
    if(this->somethingInBox()){
        /* se c'è qualcosa selezionato dall'utente con lo square */
        flag = PROPERTY_SHOW_DELETE | PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT;
    }else{
        if(m_copy->isEmpty())
            /* se non c'è niente in lista e niente selezione dall'utente */
            flag = ~(PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT | PROPERTY_SHOW_DELETE);
        else
            /* se c'è qualcosa copiato ma niente selezione dall'utente */
            flag = (PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT);
    }

    return flag;
}

#endif // SQUARE_H
