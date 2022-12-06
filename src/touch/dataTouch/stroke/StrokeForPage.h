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
    class StrokeNormal *_data;

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

    int save(WZipWriterSingle& writer) const;
    void scale(const QPointF& delta);
    void reset();
    friend class page_file;
    friend class Page;
};
