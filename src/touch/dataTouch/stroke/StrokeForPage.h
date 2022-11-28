#pragma once

#include "touch/dataTouch/datastruct/utils_datastruct.h"
#include "touch/dataTouch/point.h"
#include <QPainter>
#include <QPen>
#include "core/WList.h"
#include "StrokeNormal.h"

class StrokeForPage {
private:
    StrokeNormal *_data;

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

    void reset();
    friend class page_file;
    friend class Page;
};

inline void StrokeForPage::append(const point_s &point, pressure_t pressure)
{
    _data->append(point, pressure);
}

inline void StrokeForPage::reset()
{
    delete this->_data;
    this->_data = new StrokeNormal;
}

inline void StrokeForPage::setMetadata(const colore_s &colore)
{
    _data->setMetadata(-1, colore);
}

