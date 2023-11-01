#pragma once

#include "currenttitle/document.h"
#include <pthread.h>
#include "RubberMethod.h"
#include "touch/tools/Tools.h"
#include "Scheduler/Scheduler.h"

#define DEFAULT_GOMMA_SIZE 5

class RubberMethod: public Tools
{
private:
    static constexpr auto debug = false;
    
    /**
     * Return true if the subimage Rect (from, to) is not null
     * */
    static bool is_image_not_null(const Page &page,
                           const PointF &from, const PointF &to, int delta);

    int _base;
    PointSettable _last;
    WListFast<WVector<int>> _data_to_remove;

    WVector<Scheduler::Ptr<WTask>> _tasks;
    WVector<DataPrivateMuThread> _dataTasks;
    void (*functionToCall)(DataPrivateMuThread &);
public:
    enum type_rubber: int{
        total, /* delete all the point with the saim id */
        partial /* delete what the user touch with the pen */
    };

    RubberMethod(const type_rubber &type, const int &size_rubber);
    ~RubberMethod();
    [[nodiscard]] bool is_set() const { return this->_last.isSet(); };

    auto touchBegin(const PointF &point, double press, Document &doc) -> UpdateEvent final;
    auto touchUpdate(const PointF &point, double press, Document &doc) -> UpdateEvent final;
    auto touchEnd(const PointF& point, Document &doc) -> UpdateEvent final;

    void reset();

    // TODO -> disable copy operator

private:
    const int &_size_gomma;
    const type_rubber &_rubber_type;
};

inline void RubberMethod::reset()
{
    _base = -1;
    _last.set(false);
    _data_to_remove.clear();
}
