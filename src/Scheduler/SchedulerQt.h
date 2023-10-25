#pragma once

#include <QObject>
#include <QEvent>
#include <QGuiApplication>
#include "utils/WCommonScript.h"
#include <QThread>
#include "core/DebugVariable.h"

class SchedulerEvent: public QEvent
{
    Fn<void()> _method;
    constexpr static auto id = 2000;
    static_assert(id > QEvent::User and id < QEvent::MaxUser);
    DebugVariable<bool> _alreadyExecuted;
public:
    /**
     * \brief Constructor of the class
     * \param method Method that needs to be executed in the main thread
     * */
    explicit SchedulerEvent(Fn<void()> method)
        : QEvent(static_cast<QEvent::Type>(2000))
        , _method(std::move(method))
        , _alreadyExecuted(false)
    {

    }

    /**
     * \brief This method will call the associated method passed as parameter
     *  It's possible to call this method only once
     * */
    void run()
    {
        const auto thisId = QThread::currentThreadId();
        const auto mainThreadId = QGuiApplication::instance()->thread()->currentThreadId();
        W_ASSERT(thisId == mainThreadId);
        W_ASSERT(_alreadyExecuted == false);

        _method();
        _alreadyExecuted = true;
    }
};

class SchedulerQt: public QObject
{
    Q_OBJECT
public:
    explicit SchedulerQt(QObject *parent);
    ~SchedulerQt() override = default;

    bool eventFilter(QObject *watched, QEvent *event) override;
};