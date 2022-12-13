#pragma once

#include "core/WZipReaderSingle.h"
#include "core/WZipWriterSingle.h"
#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"


class StrokeForPage {
private:
    std::shared_ptr<class StrokeNormal> _data;

    void rep() const;

#ifdef ALL_VERSION
    void append(const StrokeNormal &stroke);
#endif // ALL_VERSION
public:
    StrokeForPage();
    ~StrokeForPage() = default;
    void setMetadata(const colore_s &colore);
    void append(const point_s &point, pressure_t pressure);

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
