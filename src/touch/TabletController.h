#pragma once

#include <QObject>

#include "touch/highlighter/Highligter.h"
#include "touch/laser/Laser.h"
#include "touch/pen/Pen.h"
#include "touch/rubber/Rubber.h"

class TabletController : public QObject
{
    Q_OBJECT
private:
    struct {
        Highligter *_highligter;
        Pen *_pen;
        Rubber *_rubber;
        Laser *_laser;
    } _tools;

    Tools *_currentTool;

    QColor _color;
    QPen _pen;

    Document *_doc;

    void objectMove();
    void callUpdate();
    void setAndCallTool(Tools *tool);

public:
    explicit TabletController(QObject *parent, const std::function<int()>& getTimeRecording);



public slots:
    void selectRubber();
    void selectPen();
    void selectSquare();
    void selectLaser();

    void touchBegin(const QPointF &point, double pressure);
    void touchUpdate(const QPointF &point, double pressure);
    void touchEnd(const QPointF &point, double pressure);

signals:
    void onNeedRefresh();
    void onToolChanged();
};

