#pragma once

#include "core/WImage.h"
#include "core/WZipReaderSingle.h"
#include "core/WZipWriterSingle.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/Point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"
#include "core/WPixmap.h"

class StrokeForPage {
private:
    std::shared_ptr<class StrokeNormal> _data;
    mutable WPixmap _pxm;
    mutable bool _needToUpdate;

    void rep() const;

    void append(const StrokeNormal &stroke);

public:
    StrokeForPage();
    ~StrokeForPage() = default;
    void setMetadata(const colore_s &colore);
    void append(const Point &point, pressure_t pressure);

    void setPressure(pressure_t press);

    int load(WZipReaderSingle &reader, int ver_stroke);

    size_t getSizeInFile() const;

    void draw(QPainter &painter, double zoom, double delta,
              QPen &pen, const QPointF &pointFirstPage,
              const class Page &page) const;

    int save(WZipWriterSingle& writer) const;
    void scale(const QPointF& delta);
    void reset();

    StrokeForPage &operator=(const StrokeForPage &other);

    friend class page_file;
    friend class Page;
};
