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
        laser = BIT(4)
    };

    UpdateEvent(WFlags<UpdateEventType> flags) : _flags(std::move(flags)), _low(0), _high(0) {};

    static auto makePage(int low, int high) { return UpdateEvent(UpdateEventType::page, low, high); }
    static auto makePageAll() -> UpdateEvent;
    static auto makeSheet() -> UpdateEvent;
    static auto makeStroke() -> UpdateEvent;

    static auto makeAll() -> UpdateEvent { return makePageAll() | makeStroke() | makeSheet(); }

    auto operator|(const UpdateEvent& d) const -> UpdateEvent { return {_flags | d._flags}; }
    auto operator&(const WFlags<UpdateEventType> &f) const -> WFlags<UpdateEventType> { return _flags & f; }


private:
    WFlags<UpdateEventType> _flags;
    int _low, _high;
    bool _all;

    UpdateEvent() = default;

    explicit UpdateEvent(WFlags<UpdateEventType> flags, int low, int high): _flags(std::move(flags)), _low(low), _high(high), _all(false) {};
};

inline auto UpdateEvent::makeSheet() -> UpdateEvent
{
    return {UpdateEventType::stroke};
}

inline auto UpdateEvent::makeStroke() -> UpdateEvent
{
    return {UpdateEventType::stroke};
}

inline auto UpdateEvent::makePageAll() -> UpdateEvent
{
    UpdateEvent event;

    event._flags = UpdateEventType::page;
    event._low = 0;
    event._high = 0;
    event._all = true;

    return event;
}
