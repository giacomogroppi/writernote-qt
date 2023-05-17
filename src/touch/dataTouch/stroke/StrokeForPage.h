#pragma once

#include "core/WImage.h"
#include "core/WZipReaderSingle.h"
#include "core/WZipWriterSingle.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/Point.h"
#include "core/WList.h"
#include "core/WImage.h"
#include "core/WPixmap.h"
#include "core/Point.h"
#include "core/WPainter/WPainter.h"

class StrokeForPage {
private:
    std::shared_ptr<class StrokeNormal> _data;

    void rep() const;

    void append(const StrokeNormal &stroke);
    mutable WPixmap _pix;

    mutable bool _needToUpdate = true;
    void draw() const;
public:
    StrokeForPage();
    ~StrokeForPage() = default;
    void setMetadata(const colore_s &colore);
    void append(const PointF &point, pressure_t pressure);

    void setPressure(pressure_t press);

    int load(WZipReaderSingle &reader, int ver_stroke);

    size_t getSizeInFile() const;

    void draw(WPainter &painter, double delta, const class Page &page, const WSizeF &target, const RectF &visibleArea) const;

    int save(WZipWriterSingle& writer) const;
    void scale(const PointF& delta);
    void reset();

    StrokeForPage &operator=(const StrokeForPage &other);

    friend class page_file;
    friend class Page;
};
