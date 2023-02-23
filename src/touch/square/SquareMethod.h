#pragma once

#include "touch/tools/Tools.h"
#include "currenttitle/document.h"
#include "touch/property/property_control.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/square/SquareMethod.h"
#include "touch/multi_thread_data.h"

class SquareMethod: public Tools, public QObject {
    Q_OBJECT

public:
    explicit SquareMethod(QObject *parent, class property_control *property);
    ~SquareMethod() override;

    bool touchBegin(const QPointF& point, double size, class Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;

    void adjustPoint();
    void needReload(QPainter &painter);
    [[nodiscard]] bool somethingInBox() const;

    /* definizione per i punti di spostamento */
    PointSettable _lastpoint;

    void initPoint(const QPointF &point);


private:
    void findObjectToDrawImg();
    void findObjectToDraw(const QList<QVector<int> > &index);
    void initImg();
    /**
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
    [[nodiscard]] int calculate_flags() const;

private slots:
    void actionProperty(property_control::ActionProperty action);

    void updatePoint(const QPointF &puntofine);
};

inline int SquareMethod::calculate_flags() const
{
    int flag = 0;

    if (this->somethingInBox()) {
        flag = PROPERTY_SHOW_DELETE | PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT;
    } else {
        if(!_copy->isEmpty()){
            flag = PROPERTY_SHOW_PASTE;
        }
    }

    W_ASSERT(flag >= 0);

    return flag;
}

