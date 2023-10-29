#pragma once

#include <utility>

#include "Scheduler/WObject.h"
#include "core/WSemaphore.h"
#include "utils/WCommonScript.h"
#include "core/WMutex.h"
#include "core/AtomicSafe.h"

class WTask: public WObject
{
private:
    std::mutex _waiterLock;
    std::condition_variable _conditionalVariable;
    bool _deleteLater;
    AtomicSafe<bool> _hasFinish;
    std::atomic<int> _threadsCreated;
public:
    // TODO: add documentation
    // TODO: change destroyLater parameter with a WFlag (for a better code reading)
    explicit WTask(WObject *parent, bool destroyLater);
    ~WTask() override;

    virtual void run() = 0;
    void join();

    void releaseJoiner() noexcept;

    constexpr void setDestroyLater(bool needToDestroy);

    /**
     * \return True iff you should delete this task after call "run"
     * */
    [[nodiscard]]
    constexpr auto isDeleteLater() const -> bool;

    WDISABILE_COPY(WTask);

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task);

    friend class Scheduler;
};

inline constexpr auto WTask::isDeleteLater() const -> bool
{
    return this->_deleteLater;
}

inline constexpr void WTask::setDestroyLater(bool needToDestroy)
{
    this->_deleteLater = needToDestroy;
}

class WTaskFunction: public WTask
{
private:
    Fn<void()> _method;
public:
    WTaskFunction (WObject *parent, bool destroyLater, Fn<void()> method)
        : WTask(parent, destroyLater)
        , _method(std::move(method))
        {};
    ~WTaskFunction() override = default;

    void run () final
    {
        _method();
    }
};