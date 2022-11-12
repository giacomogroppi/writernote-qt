#pragma once

#include <QWidget>
#include "currenttitle/document.h"
#include <QPainter>
#include <QPen>
#include <pthread.h>
#include "RubberMethod.h"

#define DEFAULT_GOMMA_SIZE 5

class RubberMethod
{
private:
    bool is_image_not_null(int index, const Page *page,
                           const QPointF &from, const QPointF &to, int delta);

    int _base;
    PointSettable _last;
    QList<QVector<int>> _data_to_remove;
public:
    RubberMethod();
    ~RubberMethod();
    [[nodiscard]] bool is_set() const { return this->_last.isSet(); };

    enum type_rubber: int{
        total, /* delete all the point with the saim id */
        partial /* delete what the user touch with the pen */
    };

    void actionRubber(const QPointF &point, type_rubber method, int size_gomma);
    void initRubber(const QPointF &point);
    int endRubber(type_rubber method);

    void reset();
};

inline void RubberMethod::reset()
{
    _base = -1;
    _last.setSet(false);
    _data_to_remove.clear();
}
