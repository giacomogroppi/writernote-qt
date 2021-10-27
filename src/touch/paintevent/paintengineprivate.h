#ifndef PAINTENGINEPRIVATE_H
#define PAINTENGINEPRIVATE_H

#include <QThread>
#include "../datastruct/point.h"
#include "../../currenttitle/document.h"

struct Point;
class MainWindow;
//#include "../tabletcanvas.h"


class PaintEnginePrivate: public QThread
{
    Q_OBJECT
public:
    static void load(QPainter &painter,
                const Document *data,
                QColor &m_color,
                QPen &m_pen,
                QBrush &m_brush,
                Point &lastPoint,
                int m_pos_ris,
                QPixmap *m_pixmap,
                const bool withPdf,
                const double m,
                const int size_orizzontale,
                const int size_verticale,
                const MainWindow *parent,
                const bool IsExportingPdf);

    PaintEnginePrivate();
    void setData(QList<point_s> & data_ref);
protected:
    void run();
};

#endif // PAINTENGINEPRIVATE_H
