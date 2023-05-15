#pragma once

#include <iostream>
#include "core/PointF.h"
#include "core/WList.h"
#include "core/Rect.h"

class Stroke;
class DataStruct;

class copy
{
public:
    copy();
    ~copy();

#define SELECTION_FLAGS_COPY 0
#define SELECTION_FLAGS_CUT 1
#define SELECTION_FLAGS_PASTE 2

    int selection(DataStruct &data,
                  const WList<
                        WList<
                            std::shared_ptr<Stroke>
                        >
                    > &stroke,
                  int __flags, const PointF &offsetTouch);


    void past_selection(DataStruct &data, PointF &point_past);

    bool isEmpty() const;

    bool isSomeThingCopy() const;
    bool isSomeThingCut() const;

    void reset();

private:
    void managePaste(DataStruct &data, const PointF &pointTouch);
    void adjustData(const PointF &offset);

#define FLAG_CUT 0x2 /* if the point is from a cut operation */
    int flags = 0;

    WList<std::shared_ptr<Stroke>> m_stroke;

    void single(const WList<std::shared_ptr<Stroke>> &from, WList<std::shared_ptr<Stroke>> &append_data);
    Rect get_size_area(const WList<WList<std::shared_ptr<Stroke>>> & stroke);
};

inline bool copy::isSomeThingCopy() const
{
    return !(this->flags & FLAG_CUT);
}

inline bool copy::isSomeThingCut() const
{
    return this->flags & FLAG_CUT;
}

inline bool copy::isEmpty() const
{
    return m_stroke.isEmpty();
}
