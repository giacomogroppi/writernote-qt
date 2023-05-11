#pragma once

#ifdef USE_QT
# include <QPixmap>
#endif // USE_QT
#include "utils/WCommonScript.h"
#include "WByteArray.h"

class WPixmap
{
public:
    explicit WPixmap(int page, bool consideringResolution);
    WPixmap() = default;
    explicit WPixmap(const std::string &path, const char *format = nullptr);

    size_t get_size_in_file() const;
    size_t save_and_size(WByteArray &arr) const;

    bool operator==(const WPixmap &other) const;
#ifdef USE_QT
    WPixmap& operator=(const QPixmap &other);
#endif
};

#ifdef USE_QT
force_inline bool WPixmap::operator==(const WPixmap &other) const
{
    return QPixmap::toImage().operator==(other.toImage());
}

inline WPixmap &WPixmap::operator=(const QPixmap &other)
{
    QPixmap::operator=(other.copy());
    return *this;
}
#endif