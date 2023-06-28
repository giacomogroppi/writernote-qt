#pragma once

#include <utility>

#include "Scheduler/WObject.h"
#include "core/WSemaphore.h"
#include "utils/WCommonScript.h"

class WTask: public WObject
{
private:
    int _waiter;
    WSemaphore _sem;
    bool _deleteLater;
public:
    explicit WTask(WObject *parent = nullptr);
    ~WTask() override = default;

    virtual void run() = 0;
    void join();

    void releaseJoiner() noexcept;

    void setDestroyLater(bool needToDestroy);

    /**
     * \return True iff you should delete this task after call "run"
     * */
    auto isDeleteLater() const -> bool;

    WDISABILE_COPY(WTask);

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task)
};

inline auto WTask::isDeleteLater() const -> bool
{
    return this->_deleteLater;
}

inline void WTask::setDestroyLater(bool needToDestroy)
{
    this->_deleteLater = needToDestroy;
}

class WTaskFunction final: public WTask {
private:
    std::function<void()> _method;
public:
    WTaskFunction (WObject *parent, std::function<void()> method) : WTask(parent), _method(std::move(method)) {};
    ~WTaskFunction() final = default;

    void run () final
    {
        _method();
    }
};
