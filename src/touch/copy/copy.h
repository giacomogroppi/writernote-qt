#ifndef COPY_H
#define COPY_H

#include <QObject>
#include "../datastruct/datastruct.h"
#include <QPointF>

class copy : public QObject
{
    Q_OBJECT
public:
    explicit copy(QObject *parent = nullptr);

    void copy_selection(datastruct *data, QPointF &topleft, QPointF &bottonright);
    void past_selection(datastruct *data, QPointF &point_past);

    bool isSomeThingCopy();

signals:

private:
    void adjastTranslation();

    datastruct m_data;

};

#endif // COPY_H
