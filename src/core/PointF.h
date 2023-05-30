//
// Created by Giacomo Groppi on 01/05/23.
//

#ifndef WRITERNOTE_POINTF_H
#define WRITERNOTE_POINTF_H

#include "PointTemplate.h"
#include "utils/WCommonScript.h"

#ifdef USE_QT
# include <QDebug>
#endif // USE_QT

using PointF = PointTemplate<double>;

#ifdef USE_QT
inline Q_CORE_EXPORT QDebug operator<<(QDebug d, const PointF &p)
{
    d.nospace() << "PointF(" << p.x() << ',' << p.y() << ')';
    return d.space();
}

#endif // USE_QT

namespace WCommonScript {
    // TODO: move this function into static PointTemplate function

    force_inline bool is_near(const PointF &point1, const PointF &point2, cdouble prec) {
        return is_near(point1.x(), point2.x(), prec) and
               is_near(point1.y(), point2.y(), prec);
    }

    force_inline double distance_not_square(const PointF &first, const PointF &second) {
        const auto p =
                WCommonScript::Power(first.x() - second.x(), 2) + WCommonScript::Power(first.y() - second.y(), 2);
        if constexpr (WCommonScript::debug_enable()) {
            const auto not_used res =
                    WCommonScript::Power(first.x() - second.x(), 2) + std::pow(first.y() - second.y(), 2);
            W_ASSERT(WCommonScript::is_near(res, p, 0.001));
        }

        return p;
    }

    force_inline double distance(const PointF &first, const PointF &second) {
        return std::sqrt(WCommonScript::distance_not_square(first, second));
    }
}


#endif //WRITERNOTE_POINTF_H
