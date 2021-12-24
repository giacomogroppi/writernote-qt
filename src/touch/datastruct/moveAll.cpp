#include "datastruct.h"
#include <QPointF>
#include "frompdf/frompdf.h"
#include "images/fromimage.h"

void datastruct::inverso(QPointF &point){
    point *= -1.0;
}

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
    this->zoom = 1.0;

    datastruct::inverso(point);

    scala_all(point, INT_MAX);
}

void datastruct::restoreLastTranslation(const int heightView){
    if(__last_translation == QPointF(0, 0))
        return;

    datastruct::inverso(__last_translation);

    scala_all(__last_translation, heightView);

    __last_translation = QPointF(0, 0);
}

void datastruct::scala_all(const QPointF &point, const int heightView)
{
    if(point == QPointF(0, 0))
        return;

    this->pointFirstPage += point;
    this->pageVisible = -1;

#ifdef PDFSUPPORT
    if(m_pdf)
        m_pdf->translation(point);
#endif

    m_img->move(point);

    if(heightView > 0)
        this->triggerVisibility(heightView);
}
