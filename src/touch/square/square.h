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
    explicit square(QObject *parent, class property_control *property);
    ~square() override;

    void reset() override;
};
