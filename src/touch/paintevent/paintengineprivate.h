#ifndef PAINTENGINEPRIVATE_H
#define PAINTENGINEPRIVATE_H

#include <QThread>
#include "../datastruct/point.h"

class PaintEnginePrivate: public QThread
{
    Q_OBJECT
public:
    PaintEnginePrivate();
    void setData(QList<point_s> & data_ref);
protected:
    void run();
};

#endif // PAINTENGINEPRIVATE_H
