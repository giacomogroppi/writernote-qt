#pragma once

#include <QObject>

#include "touch/highlighter/Highligter.h"
#include "touch/laser/Laser.h"
#include "touch/pen/Pen.h"
#include "touch/rubber/Rubber.h"
#include "object_finder/ObjectFinder.h"
#include "touch/square/Square.h"
#include "core/WImage.h"

class TabletController : public QObject
{
    Q_OBJECT
private:
    struct {
        Highligter *_highligter;
        Pen *_pen;
        Rubber *_rubber;
        Laser *_laser;
        Square *_square;
    } _tools;

    ObjectFinder *_objectFinder;
    Tools *_currentTool;
    QColor _color;
    QPen _pen;
    Document *_doc;

    mutable WImage _img;
    mutable bool _needUpdate;

    void objectMove(const QPointF &point);
    void callUpdate();
    void setAndCallTool(Tools *tool);

    Document &getDoc();

    const std::function<bool()> _isPlaying;
    const std::function<int()> _getTimePlaying;

public:
    explicit TabletController(QObject *parent,
                              const std::function<int()>& getTimeRecording,
                              const std::function<bool()> &isPlaying,
                              const std::function<int()> &getTimePlaying);

    const QImage &getImg();

private:
    void checkCreatePage();
    void draw();

public slots:
    void selectRubber();
    void selectPen();
    void selectHighligter();
    void selectSquare();
    void selectLaser();

    void selectColor(const QColor &color);

    void positionDocChanged(const QPointF &newPosition);

    void touchBegin(const QPointF &point, double pressure);
    void touchUpdate(const QPointF &point, double pressure);
    void touchEnd(const QPointF &point, double pressure);

signals:
    void onNeedRefresh();
    void onToolChanged();
    void onPropertyHide();
    void onPropertyShow(const QPointF &point, ActionProperty);
};

