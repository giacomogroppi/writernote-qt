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
    const bool _deleteLater;
    AtomicSafe<bool> _hasFinish;
public:
    // TODO: add documentation
    // TODO: change destroyLater parameter with a WFlag (for a better code reading)

    enum Flag {
        DeleteLater = 0x1,
        NotDeleteLater = 0x2
    };

    explicit WTask(WObject *parent, WFlags<Flag> destroyLater);
    ~WTask() override;

    void beforeRun();

    virtual void run() = 0;
    void join();

    void releaseJoiner() noexcept;

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

class WTaskFunction: public WTask
{
private:
    Fn<void()> _method;
public:
    WTaskFunction (WObject *parent, Flag destroyLater, Fn<void()> method)
        : WTask(parent, destroyLater)
        , _method(std::move(method))
        {};

    ~WTaskFunction() override = default;

    void run () final
    {
        _method();
    }
};