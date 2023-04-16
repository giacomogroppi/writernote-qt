#pragma once

#include <QPixmap>
#include "utils/WCommonScript.h"

class WPixmap : public QPixmap
{
public:
    explicit WPixmap(int page, bool consideringResolution);
    WPixmap() = default;
    explicit WPixmap(const QString &path, const char *format = nullptr);

    size_t get_size_in_file() const;
    size_t save_and_size(QByteArray &arr) const;

    Q_DECL_DEPRECATED_X("This function is very slow, so please don't use it...")
    bool operator==(const WPixmap &other) const;
    WPixmap& operator=(const QPixmap &other);
};

force_inline bool WPixmap::operator==(const WPixmap &other) const
{
    return QPixmap::toImage().operator==(other.toImage());
}

inline WPixmap &WPixmap::operator=(const QPixmap &other)
{
    QPixmap::operator=(other.copy());
    return *this;
}
