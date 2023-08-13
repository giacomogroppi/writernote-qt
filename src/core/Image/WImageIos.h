#pragma once


#include "core/RectF.h"
#include "WImagePrivateCommon.h"
#include "core/String/WString.h"
#include "core/PointF.h"
#include "core/Point.h"
#include "core/WRgb.h"
#include "touch/dataTouch/Point.h"
#include "core/Pointer/UniquePtr.h"

class WImage
{
private:
    RectF _rect;
public:
    // TODO: try to make this private
    // only access should be in the bridge for ios
    std::unique_ptr<struct WImagePrivate> _d;
    
    explicit WImage(const WString &path) noexcept;
    WImage (WImage &&other) noexcept;
    WImage (const WImage &other) noexcept;
    WImage () noexcept;
    
    explicit WImage(const std::string &fileName, const char *format = nullptr);
    explicit WImage(int page, bool consideringResolution);
    WImage(int width, int height, WImageType format);

    ~WImage();
    
    [[nodiscard]]
    auto get_size_in_file() const -> size_t;
    
    [[nodiscard]]
    auto save_and_size(WByteArray &arr) const -> size_t;

    auto loadFromData(const WByteArray &data, const char *formact) -> bool;

    auto height() const -> int;
    auto width() const -> int;

    auto rect() const -> RectF;

    auto isNull() const -> bool;

    auto fill(const WColor &color) -> void;
    
    static auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, WImage>;
    static auto write (WritableAbstract &writable, const WImage &source) -> int;

    auto operator==(const WImage &other) const -> bool;
    auto operator=(WImage &&other) noexcept -> WImage &;
    auto operator=(const WImage &other) noexcept -> WImage &;

    auto getRawDataPNG() const -> WByteArray;

    friend class WPainter;

    auto toImage() const -> WImage;
    
    auto pixel(const WPoint &point) const -> WRgb;
};
