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
bool datastruct::repositioning()
{
    if(isempty())
        return false;

    const point_s * __point = &m_point.first();

    if(__point->m_x == 0.0
            && __point->m_y == 0.0)
        return false;

    QPointF point_temp(__point->m_x, __point->m_y);


    datastruct::inverso(point_temp);

    scala_all(point_temp);

    return true;
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

    const point_s * __point = & m_point.first();

    __last_translation = QPointF(__point->m_x, __point->m_y);

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

    this->scala_x(point.x());

    this->scala_y(point.y());
    this->scala_posizionefoglio(point.y());

#ifdef PDFSUPPORT
    if(m_pdf)
        m_pdf->translation(point);
#endif

    m_img->move(point);
}

void datastruct::scala_x(double scala)
{
    uint i, len;
    len = this->m_point.length();
    for(i=0; i<len; i++){
        m_point.operator[](i).m_x += scala;
    }
}

void datastruct::scala_posizionefoglio(double scala){
    unsigned int i, len;
    for(i=0, len = posizionefoglio.length(); i<len; ++i){
        posizionefoglio.operator[](i) += scala;
    }
}

void datastruct::scala_y(double scala)
{
    uint i, len;
    len = this->m_point.length();
    for(i=0; i<len; i++){
        m_point.operator[](i).m_y += scala;
    }
}
