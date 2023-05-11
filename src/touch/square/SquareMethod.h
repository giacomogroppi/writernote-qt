#pragma once

#include "touch/tools/Tools.h"
#include "currenttitle/document.h"
#include "touch/copy_cut/copy_cut_selection.h"
#include "utils/WCommonScript.h"
#include "touch/multi_thread_data.h"
#include "touch/square/SquareMethod.h"
#include "touch/multi_thread_data.h"
#include "touch/tools/Scrollable.h"
#include "utils/WCommonScript.h"

enum ActionProperty: int {
    PROPERTY_SHOW_DELETE = 0x1,
    PROPERTY_SHOW_COPY = 0x2,
    PROPERTY_SHOW_CUT = 0x4,
    PROPERTY_SHOW_PASTE = 0x8
};

enum PropertySignals: int {
    SignalDelete,
    SignalCopy,
    SignalCut,
    SignalPaste
};

class SquareMethod: public Tools, public Scrollable
{
public:
    explicit SquareMethod(std::function<void()> hideProperty,
                          std::function<void(const QPointF& point, ActionProperty signal)> showProperty,
                          std::function<Document &()> getDoc);
    ~SquareMethod() ;

    int touchBegin(const QPointF& point, double size, class Document &doc) final;
    int touchUpdate(const QPointF& point, double size, class Document &doc) final;
    int touchEnd(const QPointF& point, class Document &doc) final;

    void adjustPoint();
    void needReload(QPainter &painter, const Document &doc);
    [[nodiscard]] bool somethingInBox() const;

    /* definizione per i punti di spostamento */
    PointSettable _lastpoint;

    void translate(const QPointF &point) final;
    void changeInstrument();

private:
    static constexpr auto debugSquare = false;
    int endMoving(Document &doc);

    void move(const QPointF &punto, Document &doc);
    std::function<void()> _hideProperty;
    std::function<void(const QPointF& point, ActionProperty signal)> _showProperty;
    std::function<Document &()> _getDoc;

    void initPointSearch(const QPointF &point, const Document &doc);
    void initPointMove(const QPointF &point, const Document &doc);

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
    void mergeImg(const WPixmap &from, WPixmap &to, int page);
    void moveObjectIntoPrivate(QList<QVector<int> > &index, Document &doc);

    WPixmap _img;
    QList<QList<std::shared_ptr<Stroke>>> _stroke;

    int _base;
    QList<int> _index_img; /* image */

    QPen _penna;

    bool _in_box;
    copy *_copy;

    pthread_t *_thread;
    DataPrivateMuThread *_dataThread;
    int _threadCount;

    QPointF _trans_img;
    [[nodiscard]] ActionProperty calculate_flags() const;

    void updatePoint(const QPointF &puntofine, const Document &doc);

    bool find(Document &doc);

protected:
    virtual void needRefreshPrivate() = 0;
    virtual void reset();
    void actionProperty(PropertySignals action);
};

inline ActionProperty SquareMethod::calculate_flags() const
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

    return static_cast<ActionProperty>(flag);
}

inline void SquareMethod::translate(const QPointF &)
{
    WDebug(debugSquare, "Scroll");
    //this->_property->Hide();
}

inline void SquareMethod::changeInstrument()
{
    this->reset();
    this->_hideProperty();
}

force_inline bool SquareMethod::somethingInBox() const
{
    return this->_in_box;
}
