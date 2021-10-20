#include "datastruct.h"
#include <QPointF>
#include "../../frompdf/frompdf.h"
#include "../../images/fromimage.h"

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

    QPointF point = this->pointFirstPage;


    datastruct::inverso(point);

    scala_all(point);
}

void datastruct::restoreLastTranslation(){
    if(__last_translation == QPointF(0, 0))
        return;

    datastruct::inverso(__last_translation);

    scala_all(__last_translation);

    __last_translation = QPointF(0, 0);
}

void datastruct::scala_all()
{
    if(isempty()){
        this->__last_translation = QPointF(0, 0);
        return;
    }

    __last_translation = this->getPointFirstPage();

    datastruct::inverso(__last_translation);

    scala_all(__last_translation);
}

/*
 * pass m_pdf only if you want to translate also the pdf
 */
void datastruct::scala_all(const QPointF &point)
{
    if(point == QPointF(0, 0))
        return;

    this->pointFirstPage += point;

#ifdef PDFSUPPORT
    if(m_pdf)
        m_pdf->translation(point);
#endif

    m_img->move(point);
    this->triggerVisibility();
}
