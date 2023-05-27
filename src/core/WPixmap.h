#pragma once

#ifdef USE_QT
# include <QPixmap>
#endif // USE_QT
#include "utils/WCommonScript.h"
#include "WByteArray.h"
#include "RectF.h"
#include "WImage.h"
#include "WString.h"

class WPixmap
{
private:
#ifdef USE_QT
    QPixmap _pixmap;
#endif // USE_QT
public:
    explicit WPixmap(int page, bool consideringResolution);
    WPixmap(const WString &path);
    WPixmap() = default;
    explicit WPixmap(const std::string &path, const char *format = nullptr);

    size_t get_size_in_file() const;
    size_t save_and_size(WByteArray &arr) const;

    void fill(const WColor &color);
    RectF rect() const;
    WImage toImage() const;
    bool isNull() const;

    bool loadFromData(const WByteArray& data, const char *formact);
    bool loadFromData(const void *data, size_t size, const char *formact);

    bool operator==(const WPixmap &other) const;
#ifdef USE_QT
    WPixmap& operator=(const QPixmap &other);
    QPixmap &getPixmap();
#endif
};

#ifdef USE_QT
force_inline bool WPixmap::operator==(const WPixmap &other) const
{
    return _pixmap.toImage().operator==(other.toImage());
}

inline WPixmap &WPixmap::operator=(const QPixmap &other)
{
    _pixmap.operator=(other.copy());
    return *this;
}

inline QPixmap &WPixmap::getPixmap()
{
    return this->_pixmap;
}
#endif
