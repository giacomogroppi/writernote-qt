#pragma once

#include "currenttitle/document.h"
#include <QPainter>
#include <QPen>
#include <pthread.h>
#include "RubberMethod.h"
#include "touch/tools/Tools.h"

#define DEFAULT_GOMMA_SIZE 5

class RubberMethod: public Tools
{
private:
    /**
     * Return true if the subimage QRect (from, to) is not null
     * */
    static bool is_image_not_null(const Page *page,
                           const QPointF &from, const QPointF &to, int delta);

    int _base;
    PointSettable _last;
    QList<QVector<int>> _data_to_remove;
public:
    enum type_rubber: int{
        total, /* delete all the point with the saim id */
        partial /* delete what the user touch with the pen */
    };

    RubberMethod(const type_rubber &type, const int &size_rubber);
    ~RubberMethod();
    [[nodiscard]] bool is_set() const { return this->_last.isSet(); };

    bool touchBegin(const QPointF &point, double press, Document &doc) final;
    bool touchUpdate(const QPointF &point, double press, Document &doc) final;
    int touchEnd(const QPointF& point, Document &doc) final;

    void reset();

    Q_DISABLE_COPY(RubberMethod);

private:
    const int &_size_gomma;
    const type_rubber &_rubber_type;
};

inline void RubberMethod::reset()
{
    _base = -1;
    _last.setSet(false);
    _data_to_remove.clear();
}
