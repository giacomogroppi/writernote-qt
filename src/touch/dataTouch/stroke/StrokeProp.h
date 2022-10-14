#pragma once

#include "utils/common_script.h"
#include <QString>

class StrokeProp {
public:
    enum flag_complex : int{
        COMPLEX_NORMAL = 0,
        COMPLEX_CIRCLE = 1,
        COMPLEX_RECT = 2,
        COMPLEX_LINE = 3
    };

private:
    flag_complex _prop;
    static_assert(sizeof(_prop) == sizeof(int));

#ifdef DEBUGINFO
    static void checkType(int type);
#else
    static void checkType(int type) {};
#endif
public:
    explicit StrokeProp (flag_complex type);
    ~StrokeProp() = default;

    static_assert_type(_prop, flag_complex);
    static_assert(sizeof(_prop) == sizeof(int));

    force_inline bool is_normal() const { return _prop == COMPLEX_NORMAL; };
    force_inline bool is_circle() const { return _prop == COMPLEX_CIRCLE; };
    force_inline bool is_rect() const { return _prop == COMPLEX_RECT; };
    force_inline bool is_line() const { return _prop == COMPLEX_LINE; };
    force_inline bool is_complex() const { return _prop != COMPLEX_NORMAL; };

    [[nodiscard]] typeof(_prop) getProp() const { return _prop; };
    [[nodiscard]] typeof(_prop) &PropRef() { return _prop; };
    [[nodiscard]] QString toString() const noexcept;

protected:
    void setProp (const StrokeProp &prop) noexcept;
    void setProp (flag_complex type) noexcept;
};

Q_CORE_EXPORT QDataStream &operator<<(QDataStream &d, const StrokeProp &str);

#ifdef DEBUGINFO
inline QString StrokeProp::toString() const noexcept
{
    switch (_prop) {
        case COMPLEX_NORMAL:
            return qstr("Stroke Normal");
        case COMPLEX_LINE:
            return qstr("Stroke Line");
        case COMPLEX_CIRCLE:
            return qstr("Stroke Circle");
        case COMPLEX_RECT:
            return qstr("Stroke Rect");
    }
    std::abort();
}
#endif // DEBUGINFO

inline void StrokeProp::setProp(flag_complex type) noexcept
{
    StrokeProp::checkType(type);
    this->_prop = type;
}

inline StrokeProp::StrokeProp(flag_complex type)
{
    StrokeProp::checkType(type);
    this->_prop = type;
}
