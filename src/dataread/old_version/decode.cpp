#include "../xmlstruct.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"

static void scaleAll(QList<point_s> &point, const QPointF &translation);
static void adjastZoom(QList<point_s> &point, QList<double> &pos_foglio);
static QPointF biggerx(const QList<point_s> &point);
static void adjastPDF(QList<point_s> &point, QList<double> &pos_foglio);

void xmlstruct::decode(Document *data, QList<point_s> &point, QList<double> &pos_foglio)
{
    const point_s firstPoint = point.takeFirst();
    const uint lenPage = pos_foglio.length();
    const point_s *pointAppend;
    const QPointF translation(firstPoint.m_x, firstPoint.m_y);
    const uint height = page::getHeight();

    uint i=0, counterPage;

    scaleAll(point, translation);
    adjastZoom(point, pos_foglio);
    data->datatouch->setPointFirstPage(translation);

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        data->datatouch->newPage(n_style::white);
    }

    for(counterPage = 0; counterPage < lenPage ; counterPage ++){


        uint lenList = point.length();
        for(i=0; i<lenList; ++i){

            pointAppend = &point.at(i);

            if(pointAppend->m_y > counterPage*height && pointAppend->m_y <= (counterPage+1)*height){
                data->datatouch->append(pointAppend);
            }
        }
    }
}

static void scaleAll(QList<point_s> &point, const QPointF &translation)
{
    uint i;
    const uint len = point.length();
    point_s *ref;
    for(i=0; i<len; i++){
        ref = &point.operator[](i);
        ref->m_x -= translation.x();
        ref->m_y -= translation.y();
    }
}

static void adjastZoom(QList<point_s > &point, QList<double> &pos_foglio)
{
    const QPointF bigger = biggerx(point);
    const double Width = page::getWidth();
    const double delta = Width / bigger.x();

    point_s *ref;

    uint i;
    uint len = point.length();

    for(i = 0; i < len; i++){
        ref = &point.operator[](i);
        ref->m_x *= delta;
        ref->m_y *= delta;
    }

    len = pos_foglio.length();
    for(i=0; i<len; i++){
        pos_foglio.operator[](i) *= delta;
    }

    adjastPDF(point, pos_foglio);
}

static void adjastPDF(QList<point_s> &point, QList<double> &pos_foglio){
    uint i, len;
    const QPointF currentSize = biggerx(point);
    const double CorrectProportions = double(page::getHeight())/double(page::getWidth());
    const uint lenPage = pos_foglio.length();

    const double shouldBe = currentSize.x()*CorrectProportions*lenPage;
    const double tmp = shouldBe/currentSize.y();

    len = point.length();
    for(i = 0; i < len; i++){
        point.operator[](i).m_y *= tmp;
    }

    len = pos_foglio.length();
    for(i = 0; i < len; i++){
        pos_foglio.operator[](i) *= tmp;
    }
}

static QPointF biggerx(const QList<point_s> &point)
{
    QPointF max(0, 0);
    const uint len = point.length();
    uint i;
    const point_s *ref;

    for(i = 0; i < len; i++ ){
        ref = &point.at(i);
        if(ref->m_x > max.x())
            max.setX(ref->m_x);
        if(ref->m_y > max.y())
            max.setY(ref->m_y);
    }

    return max;
}
