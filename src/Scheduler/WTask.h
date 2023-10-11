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
    bool _deleteLater;
    bool _hasFinish;
    unsigned long _identifier;
    unsigned long _threadsCreated;
public:
    explicit WTask(WObject *parent = nullptr, bool destroyLater = false);
    ~WTask() override;

    virtual void run() = 0;
    void join();

    void releaseJoiner() noexcept;

    constexpr void setDestroyLater(bool needToDestroy);

    constexpr auto getIdentifier() const noexcept -> unsigned long;

    /**
     * \return True iff you should delete this task after call "run"
     * */
    [[nodiscard]]
    auto isDeleteLater() const -> bool;

    WDISABILE_COPY(WTask);

    /**
     * @ensures task == this
     * */
    W_EMITTABLE_1(finished, WTask*, task);

    friend class Scheduler;
};

inline auto WTask::isDeleteLater() const -> bool
{
    return this->_deleteLater;
}

inline constexpr void WTask::setDestroyLater(bool needToDestroy)
{
    this->_deleteLater = needToDestroy;
}

constexpr auto WTask::getIdentifier() const noexcept -> unsigned long
{
    return _identifier;
}

class WTaskFunction: public WTask
{
private:
    Fn<void()> _method;
public:
    WTaskFunction (WObject *parent, Fn<void()> method, bool destroyLater = false)
        : WTask(parent, destroyLater)
        , _method(std::move(method))
        {};
    ~WTaskFunction() override = default;

    void run () final
    {
        _method();
    }
};