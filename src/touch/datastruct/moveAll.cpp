#include "datastruct.h"
#include <QPointF>
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

/*
 * return true if the first point is different
 * from (0.0, 0.0)
*/
void datastruct::repositioning()
{
    if(isempty())
        return;

    QPointF point = this->getPointFirstPage();
    qDebug() << "datastruct::repositioning" << point;
    this->_zoom = 1.0;

    datastruct::inverso(point);

    scala_all(point, INT_MAX);
}

void datastruct::restoreLastTranslation(const int heightView){
    if(_last_translation == QPointF(0, 0))
        return;

    datastruct::inverso(_last_translation);

    scala_all(_last_translation, heightView);

    _last_translation = QPointF(0, 0);
}

void datastruct::scala_all(const QPointF &point, const int heightView)
{
    if(point == QPointF(0, 0))
        return;

    this->_pointFirstPage += point;
    this->pageVisible = -1;

#ifdef PDFSUPPORT
    _pdf->translation(point);
#endif

    _img->move(point);

    if(likely(heightView > 0)){
        this->triggerVisibility(heightView);
    }
}
