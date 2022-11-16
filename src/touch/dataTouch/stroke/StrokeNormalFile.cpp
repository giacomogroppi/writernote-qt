#include "StrokeNormalFile.h"

#define stroke_file_size_len 4

StrokeNormalFileSave::StrokeNormalFileSave(const StrokeNormal &_stroke) :
    _data(_stroke)
{
}

size_t StrokeNormalFileSave::getSizeInFile() const
{
    size_t s = 0;
    cint len_pressure = _data._pressure.length();
    cint len_point    = _data._point.length();

    static_assert(sizeof(len_pressure) == sizeof(len_point));
    static_assert(sizeof(len_pressure) == stroke_file_size_len);

    static_assert_type(len_pressure, const int);
    static_assert_type(len_point, const int);

    s += sizeof(len_point);
    s += sizeof(len_pressure);

    s += sizeof(pressure_t)     * len_pressure;
    s += sizeof(point_s)        * len_point;

    return s;
}