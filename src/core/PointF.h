#pragma once

#include "PointTemplate.h"
#include "utils/WCommonScript.h"

#ifdef USE_QT
# include <QDebug>
#endif // USE_QT

using PointF = PointTemplate<double>;

namespace WUtils {
    // TODO: move this function into static PointTemplate function

    template <class T>
    force_inline bool is_near(const PointTemplate<T> &point1, const PointTemplate<T> &point2, T prec) {
        return is_near(point1.x(), point2.x(), prec) and
               is_near(point1.y(), point2.y(), prec);
    }

    force_inline double distance_not_square(const PointF &first, const PointF &second) {
        const auto p =
                WUtils::Power(first.x() - second.x(), 2) + WUtils::Power(first.y() - second.y(), 2);
        if constexpr (WUtils::debug_enable()) {
            const auto not_used res =
                    WUtils::Power(first.x() - second.x(), 2) + std::pow(first.y() - second.y(), 2);
            W_ASSERT(WUtils::is_near(res, p, 0.001));
        }

        return p;
    }

    force_inline double distance(const PointF &first, const PointF &second) {
        return std::sqrt(WUtils::distance_not_square(first, second));
    }
}
