#pragma once

//TODO: do some refactoring

#include <utility>

#include "core/WFlags.h"
#include "utils/WCommonScript.h"

class UpdateEvent {
public:
    enum UpdateEventType {
        page = BIT(1),
        stroke = BIT(2),
        sheet = BIT(3),
        laser = BIT(4),
        square = BIT(5)
    };

    UpdateEvent(WFlags<UpdateEventType> flags) : _flags(flags), _low(0), _high(0) {};

    static auto makePage(int low, int high) { return UpdateEvent(WFlags{UpdateEventType::page}, low, high); }
    static auto makePageAll() -> UpdateEvent;
    static auto makeSheet() -> UpdateEvent;
    static auto makeStroke() -> UpdateEvent;
    static auto makeSquare() -> UpdateEvent;
    static auto makeEmpty() -> UpdateEvent;

    static auto makeAll() -> UpdateEvent { return makePageAll() | makeStroke() | makeSheet(); }

    auto operator|(const UpdateEvent& d) const -> UpdateEvent { return {_flags | d._flags}; }
    auto operator&(const WFlags<UpdateEventType> &f) const -> WFlags<UpdateEventType> { return _flags & f; }

    /**
     * This function is defined only if the event has UpdateEventType::page and isAll() return false
    */
    auto getPageLow() const -> int;

    /**
     * This function is defined only if the event has UpdateEventType::page and isAll() return false
    */
    auto getPageHigh() const -> int;
    auto isAll() const -> bool;

private:
    WFlags<UpdateEventType> _flags;
    int _low, _high;
    bool _all = false;

    UpdateEvent() = default;

    explicit UpdateEvent(WFlags<UpdateEventType> flags, int low, int high): _flags(flags), _low(low), _high(high), _all(false) {};
};

inline auto UpdateEvent::makeEmpty() -> UpdateEvent
{
    return WFlags<UpdateEventType>{0};
}

inline auto UpdateEvent::makeSheet() -> UpdateEvent
{
    return WFlags{UpdateEventType::stroke};
}

inline auto UpdateEvent::makeStroke() -> UpdateEvent
{
    return WFlags{UpdateEventType::stroke};
}

inline auto UpdateEvent::makeSquare() -> UpdateEvent
{
    return WFlags{UpdateEventType::square};
}

inline auto UpdateEvent::makePageAll() -> UpdateEvent
{
    UpdateEvent event;

    event._flags = WFlags{UpdateEventType::page};
    event._low = 0;
    event._high = 0;
    event._all = true;

    return event;
}

inline auto UpdateEvent::getPageLow() const -> int
{
    W_ASSERT(!isAll());
    return this->_low;
}

inline auto UpdateEvent::getPageHigh() const -> int
{
    W_ASSERT(!isAll());
    return _high;
}

inline auto UpdateEvent::isAll() const -> bool
{
    return _all;
}
