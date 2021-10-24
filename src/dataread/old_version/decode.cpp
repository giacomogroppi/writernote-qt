#include "../xmlstruct.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"

static void scaleAll(QList<point_s> &point, const QPointF &translation);

void xmlstruct::decode(Document *data, QList<point_s> &point, QList<double> pos_foglio){
    const point_s firstPoint = point.takeFirst();
    const uint lenList = point.length();
    const uint lenPage = pos_foglio.length();
    const point_s *pointAppend;
    uint i, counterPage;
    const QPointF translation(firstPoint.m_x, firstPoint.m_y);
    const uint height = page::getHeight();

    scaleAll(point, translation);
    data->datatouch->setPointFirstPage(translation);

    for(counterPage = 0; counterPage < lenPage ; counterPage ++){
        data->datatouch->newPage(n_style::white);
        for(i=0; i<lenList; ++i){
            pointAppend = &point.at(i);
            if(pointAppend->m_y > counterPage*height  &&  pointAppend->m_y <= (counterPage+1)*height)
                data->datatouch->at_mod(counterPage)->append(pointAppend);
        }
    }
}

static void scaleAll(QList<point_s> &point, const QPointF &translation){
    uint i;
    const uint len = point.length();
    point_s *ref;
    for(i=0; i<len; i++){
        ref = &point.operator[](i);
        ref->m_x -= translation.x();
        ref->m_y -= translation.y();
    }
}
