#ifndef COPY_SELECTION_H
#define COPY_SELECTION_H

#include <QObject>
#include "../../currenttitle/document.h"
#include <QPointF>

class copy : public QObject
{
    Q_OBJECT
public:
    explicit copy(QObject *parent = nullptr);
    ~copy();

    void copy_selection(datastruct *data, QPointF &topleft, QPointF &bottonright);
    void past_selection(datastruct *data, QPointF &point_past);

    bool isSomeThingCopy();

signals:

private:

    datastruct *m_data;

};

#endif // COPY_SELECTION_H
