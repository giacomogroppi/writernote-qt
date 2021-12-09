#ifndef COPY_SELECTION_H
#define COPY_SELECTION_H

#include <QObject>
#include <QPointF>

class stroke;
class datastruct;

class copy : public QObject
{
    Q_OBJECT
public:
    explicit copy(QObject *parent);
    ~copy();

#define SELECTION_FLAGS_COPY 0
#define SELECTION_FLAGS_CUT 1
    void selection(datastruct &data, const QList<int> &id, int __flags);
    void past_selection(datastruct &data, QPointF &point_past);

    bool isSomeThingCopy();

    void reset();

private:
    void adjustData(const QRectF &areaData);


#define FLAG_COPY_SOME_THING_COPY 0x1
#define FLAG_CUT 0x2 /* if the point is from a cut operation */
    int flags = 0;

    QList<stroke> m_stroke;

};

#endif // COPY_SELECTION_H
