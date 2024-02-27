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

    constexpr UpdateEvent(WFlags<UpdateEventType> flags) : _flags(flags), _low(0), _high(0) {};

    static constexpr auto makePage(int low, int high) { return UpdateEvent(WFlags{page}, low, high); }
    static constexpr auto makePageAll() -> UpdateEvent;
    static constexpr auto makePageSingle(int page) -> UpdateEvent;

    static constexpr auto makeSheet() -> UpdateEvent;
    static constexpr auto makeStroke() -> UpdateEvent;
    static constexpr auto makeSquare() -> UpdateEvent;
    static constexpr auto makeLaser() -> UpdateEvent;
    static constexpr auto makeEmpty() -> UpdateEvent;

    static constexpr auto makeAll() -> UpdateEvent { return makePageAll() | makeStroke() | makeSheet() | makeSquare() | makeLaser(); }

    constexpr auto operator|(const UpdateEvent& d) const -> UpdateEvent;
    constexpr auto operator&(const WFlags<UpdateEventType> &f) const -> WFlags<UpdateEventType> { return _flags & f; }

    /**
     * This function is defined only if the event has UpdateEventType::page and isAll() return false
    */
    nd constexpr auto getPageLow() const -> int;

    /**
     * This function is defined only if the event has UpdateEventType::page and isAll() return false
    */
    nd constexpr auto getPageHigh() const -> int;
    nd constexpr auto isAll() const -> bool;

private:
    WFlags<UpdateEventType> _flags;
    int _low = -1, _high = -1;
    bool _all = false;

    constexpr UpdateEvent() = default;

    constexpr explicit UpdateEvent(WFlags<UpdateEventType> flags, int low, int high): _flags(flags), _low(low), _high(high), _all(false) {};
};

inline constexpr auto UpdateEvent::operator|(const UpdateEvent &d) const -> UpdateEvent
{
    UpdateEvent event;
    event._low = std::min(_low, d._low);
    event._high = std::max(_high, d._high);
    event._all = _all + d._all;
    event._flags = _flags | d._flags;
    return event;
}

inline constexpr auto UpdateEvent::makeEmpty() -> UpdateEvent
{
    return WFlags{static_cast<UpdateEventType>(0)};
}

inline constexpr auto UpdateEvent::makeSheet() -> UpdateEvent
{
    return WFlags{stroke};
}

inline constexpr auto UpdateEvent::makeStroke() -> UpdateEvent
{
    return WFlags{stroke};
}

inline constexpr auto UpdateEvent::makeSquare() -> UpdateEvent
{
    return WFlags{UpdateEventType::square};
}

inline constexpr auto UpdateEvent::makeLaser() -> UpdateEvent
{
    return WFlags{UpdateEventType::laser};
}

inline constexpr auto UpdateEvent::makePageAll() -> UpdateEvent
{
    UpdateEvent event;

    event._flags = WFlags{UpdateEventType::page};
    event._low = -1;
    event._high = -1;
    event._all = true;

    return event;
}

inline constexpr auto UpdateEvent::getPageLow() const -> int
{
    W_ASSERT(!isAll());
    return this->_low;
}

inline constexpr auto UpdateEvent::getPageHigh() const -> int
{
    W_ASSERT(!isAll());
    return _high;
}

inline constexpr auto UpdateEvent::isAll() const -> bool
{
    return _all;
}

inline constexpr auto UpdateEvent::makePageSingle(int page) -> UpdateEvent
{
    W_ASSERT(page >= 0);
    return UpdateEvent::makePage(page, page + 1);
}
