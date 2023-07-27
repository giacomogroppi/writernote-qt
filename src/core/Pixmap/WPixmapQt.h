#pragma once

#ifdef USE_QT

#include <QPixmap>
#include "utils/WCommonScript.h"
#include "core/ByteArray/WByteArray.h"
#include "core/RectF.h"
#include "core/Image/WImage.h"
#include "core/String/WString.h"
#include "touch/dataTouch/Point.h"

class WPixmap : public QPixmap
{
private:

public:
    WPixmap() = default;
    WPixmap (const WPixmap &other) noexcept;
    WPixmap (WPixmap &&other) noexcept;

    explicit WPixmap(int page, bool consideringResolution);
    explicit WPixmap(const WString &path);
    explicit WPixmap(const std::string &path, const char *format = nullptr);

    static size_t getSizeInFile(const WPixmap &source);
    size_t save_and_size(WByteArray &arr) const;

    void fill(const WColor &color);
    WRect rect() const;
    WImage toImage() const;
    bool isNull() const;

    auto getRawDataPNG() const -> WByteArray;
    bool loadFromData(const WByteArray& data, const char *formact);

    [[deprecated("This function is very slow, and it's deprecated")]]
    bool operator==(const WPixmap &other) const;

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WPixmap>;
    static auto write(WritableAbstract &writable, const WPixmap &pixmap) -> int;

    auto operator=(const WPixmap &other) noexcept -> WPixmap &;
    auto operator=(WPixmap &&other) noexcept -> WPixmap &;
};

#endif // USE_QT
