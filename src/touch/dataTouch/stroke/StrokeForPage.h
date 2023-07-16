#pragma once

#include "core/WImage.h"
#include "FileContainer/WZipReaderSingle.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/Point.h"
#include "core/WList.h"
#include "core/WImage.h"
#include "core/WPixmap.h"
#include "core/Point.h"
#include "core/WPainter/WPainter.h"
#include "FileContainer/WZipWriterSingle.h"
#include <memory>

class StrokeForPage {
private:
    SharedPtr<class StrokeNormal> _data;

    void rep() const;

    void append(const StrokeNormal &stroke);
    mutable WPixmap _pix;

    mutable bool _needToUpdate = true;
    void draw() const;
public:
    StrokeForPage();
    StrokeForPage(StrokeForPage &&other) noexcept;
    ~StrokeForPage() = default;
    void setMetadata(const WColor &colore);
    void append(const PointF &point, pressure_t pressure);

    void setPressure(pressure_t press);

    static
    auto load (const VersionFileController &versionController, ReadableAbstract &readable) -> WPair<int, StrokeForPage>;

    static auto write (WritableAbstract &writable, const StrokeForPage &strokeForPage) -> int;

    size_t getSizeInFile() const;

    void draw(WPainter &painter, double delta, const class Page &page, const WSizeF &target, const RectF &visibleArea) const;

    void scale(const PointF& delta);
    void reset();

    StrokeForPage &operator=(const StrokeForPage &other);
    StrokeForPage &operator=(StrokeForPage &&other) noexcept;

    auto operator==(const StrokeForPage &other) const noexcept -> bool;



    friend class page_file;
    friend class Page;
};
