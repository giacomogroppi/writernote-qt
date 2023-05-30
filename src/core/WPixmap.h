#pragma once

#ifdef USE_QT
# include <QPixmap>
#endif // USE_QT
#include "utils/WCommonScript.h"
#include "WByteArray.h"
#include "RectF.h"
#include "WImage.h"
#include "WString.h"
#include "touch/dataTouch/Point.h"

class WPixmap
#ifdef USE_QT
    : public QPixmap
#endif // USE_QT
{
private:

public:
    explicit WPixmap(int page, bool consideringResolution);
    explicit WPixmap(const WString &path);
    WPixmap() = default;
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
};
