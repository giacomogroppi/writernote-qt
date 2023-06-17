#pragma once

#ifdef USE_QT
# include <QPixmap>
#endif // USE_QT
#include "utils/WCommonScript.h"
#include "core/ByteArray/WByteArray.h"
#include "RectF.h"
#include "WImage.h"
#include "String/WString.h"
#include "touch/dataTouch/Point.h"

class WPixmap
#ifdef USE_QT
    : public QPixmap
#endif // USE_QT
{
private:

public:
    WPixmap() = default;
    WPixmap (const WPixmap &other) noexcept;
    WPixmap (WPixmap &&other) noexcept;

    explicit WPixmap(int page, bool consideringResolution);
    explicit WPixmap(const WString &path);
    explicit WPixmap(const std::string &path, const char *format = nullptr);

    size_t get_size_in_file() const;
    size_t save_and_size(WByteArray &arr) const;

    void fill(const WColor &color);
    Rect rect() const;
    WImage toImage() const;
    bool isNull() const;

    bool loadFromData(const WByteArray& data, const char *formact);

    [[deprecated("This function is very slow, and it's deprecated")]]
    bool operator==(const WPixmap &other) const;

    WPixmap &operator=(const WPixmap &other) noexcept;
    WPixmap &operator=(WPixmap &&other) noexcept;
};
