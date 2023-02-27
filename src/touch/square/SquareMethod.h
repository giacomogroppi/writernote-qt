#pragma once

#include "touch/tools/Tools.h"
#include "currenttitle/document.h"
#include "touch/property/property_control.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/square/SquareMethod.h"
#include "touch/multi_thread_data.h"
#include "touch/tools/Scrollable.h"
#include "utils/WCommonScript.h"

class SquareMethod: public Tools, public Scrollable {
private:
    static constexpr auto debugSquare = false;
    void endMoving(const QWidget *pixmap, Document &doc);

    void move(const QPointF &punto, Document &doc);

public:
    explicit SquareMethod(class property_control *property);
    ~SquareMethod() ;

    bool touchBegin(const QPointF& point, double size, class Document &doc) final;
    bool touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;

    void adjustPoint();
    void needReload(QPainter &painter, const Document &doc);
    [[nodiscard]] bool somethingInBox() const;

    /* definizione per i punti di spostamento */
    PointSettable _lastpoint;

    void initPoint(const QPointF &point, const Document &doc);
    void initPointMove(const QPointF &point, const Document &doc);

    void translate(const QPointF &point) final;
    void changeInstrument();

private:
    void findObjectToDrawImg(Document &doc);
    void findObjectToDraw(const QList<QVector<int> > &index, Document &doc);
    void initImg(const Document &doc);
    /**
     * la variabile bool viene settata a true quando c'è bisogno di disegnare
     * il rettangono
    */
    bool _need_reload = false;

    PointSettable _pointinit;
    PointSettable _pointfine;
    void mergeImg(const WImage &from, WImage &to, int page);
    void moveObjectIntoPrivate(QList<QVector<int> > &index, Document &doc);

    WImage _img;
    QList<QList<std::shared_ptr<Stroke>>> _stroke;

    int _base;
    QList<int> _index_img; /* image */

    QPen _penna;
    class property_control *_property;

    bool _in_box;
    copy *_copy;

    pthread_t *_thread;
    DataPrivateMuThread *_dataThread;
    int _threadCount;

    QPointF _trans_img;
    [[nodiscard]] int calculate_flags() const;

    void updatePoint(const QPointF &puntofine, const Document &doc);

    bool find(Document &doc);

protected:
    virtual void reset();
    void actionProperty(property_control::ActionProperty action);
};

inline int SquareMethod::calculate_flags() const
{
    int flag = 0;

    if (this->somethingInBox()) {
        flag = PROPERTY_SHOW_DELETE | PROPERTY_SHOW_COPY | PROPERTY_SHOW_CUT;
    } else {
        if (!_copy->isEmpty()) {
            flag = PROPERTY_SHOW_PASTE;
        }
    }

    W_ASSERT(flag >= 0);

    return flag;
}

inline void SquareMethod::translate(const QPointF &)
{
    WDebug(debugSquare, "Scroll");
    //this->_property->Hide();
}

inline void SquareMethod::changeInstrument()
{
    this->reset();
    this->_property->Hide();
}