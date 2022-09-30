#pragma once

#include <QPointF>
#include <QRectF>
#include <QList>

class Stroke;
class datastruct;

class copy
{
public:
    copy();
    ~copy();

#define SELECTION_FLAGS_COPY 0
#define SELECTION_FLAGS_CUT 1
#define SELECTION_FLAGS_PASTE 2
    int selection(datastruct &data, const QList<QList<Stroke> > &stroke,
                   int __flags, const QPointF &offsetTouch);
    void past_selection(datastruct &data, QPointF &point_past);

    bool isEmpty() const;

    bool isSomeThingCopy() const;
    bool isSomeThingCut() const;

    void reset();

private:
    void managePaste(datastruct &data, const QPointF &pointTouch);
    void adjustData(const QPointF &offset);

#define FLAG_CUT 0x2 /* if the point is from a cut operation */
    int flags = 0;

    QList<Stroke> m_stroke;
    void __single(const QList<Stroke> &from, QList<Stroke> &append_data);
    QRect get_size_area(const QList<QList<Stroke>> & stroke);
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
