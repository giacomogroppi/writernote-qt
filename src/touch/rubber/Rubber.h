#pragma once

#include <QObject>
#include "currenttitle/document.h"
#include <QPainter>
#include <QPen>
#include <pthread.h>
#include "RubberMethod.h"

#define POSITION_ALFA 3
#define DECREASE 2

class Rubber : public QObject,
               public RubberMethod
{
    Q_OBJECT

public:
    explicit Rubber(QObject *parent);
    virtual ~Rubber();

    int getType() const final;
    static constexpr int type();

    void setRubberTotal();
    void setRubberPartial();
signals:
    void onRubberChange();
private:
    int _size_gomma = DEFAULT_GOMMA_SIZE;
    RubberMethod::type_rubber _type_gomma = RubberMethod::total;
};


inline constexpr int Rubber::type()
{
    return 3;
}
