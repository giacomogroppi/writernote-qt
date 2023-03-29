#pragma once

#include <QPointF>
#include <QRectF>
#include <QList>

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
                  const QList<
                        QList<
                            std::shared_ptr<Stroke>
                        >
                    > &stroke,
                  int __flags, const QPointF &offsetTouch);


    void past_selection(DataStruct &data, QPointF &point_past);

    bool isEmpty() const;

    bool isSomeThingCopy() const;
    bool isSomeThingCut() const;

    void reset();

private:
    void managePaste(DataStruct &data, const QPointF &pointTouch);
    void adjustData(const QPointF &offset);

#define FLAG_CUT 0x2 /* if the point is from a cut operation */
    int flags = 0;

    QList<std::shared_ptr<Stroke>> m_stroke;

    void single(const QList<std::shared_ptr<Stroke>> &from, QList<std::shared_ptr<Stroke>> &append_data);
    QRect get_size_area(const QList<QList<std::shared_ptr<Stroke>>> & stroke);
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
