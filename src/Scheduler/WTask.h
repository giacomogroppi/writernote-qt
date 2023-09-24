#pragma once

#include <utility>

#include "Scheduler/WObject.h"
#include "core/WSemaphore.h"
#include "utils/WCommonScript.h"
#include "core/WMutex.h"

class WTask: public WObject
{
private:
    WMutex _waiterLock;
    int _waiter;
    WSemaphore _sem;
    bool _deleteLater : 1;
    bool _hasFinish : 1;
public:
    explicit WTask(WObject *parent = nullptr, bool destroyLater = false);
    ~WTask() override;

    virtual void run() = 0;
    void join();

    void releaseJoiner() noexcept;

    constexpr void setDestroyLater(bool needToDestroy);

    /**
     * \return True iff you should delete this task after call "run"
     * */
    [[nodiscard]]
    auto isDeleteLater() const -> bool;

    WDISABILE_COPY(WTask);

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task)
};

inline WTask::~WTask()
{
    WDebug(false, "Delete object" << static_cast<void*>(this));
}

inline auto WTask::isDeleteLater() const -> bool
{
    return this->_deleteLater;
}

inline constexpr void WTask::setDestroyLater(bool needToDestroy)
{
    this->_deleteLater = needToDestroy;
}

class WTaskFunction final: public WTask{
private:
    Fn<void()> _method;
public:
    WTaskFunction (WObject *parent, Fn<void()> method, bool destroyLater = false)
        : WTask(parent, destroyLater)
        , _method(std::move(method))
        {};
    ~WTaskFunction() final = default;

    void run () final
    {
        _method();
    }
};
