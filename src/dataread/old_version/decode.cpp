#include "../xmlstruct.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"

static void scaleAll(QList<point_s> &point, const QPointF &translation);
static void adjastZoom(QList<point_s> &point, QList<double> &pos_foglio);
static QPointF bigger(const QList<point_s> &point);
static void adjastPDF(QList<point_s> &point, QList<double> &pos_foglio);

void xmlstruct::decode(Document *data, QList<point_s> &point, QList<double> &pos_foglio)
{
    const point_s firstPoint = point.takeFirst();
    const int lenPage = pos_foglio.length();
    //int len;
    const QPointF translation(firstPoint.m_x, firstPoint.m_y);
    //const uint height = page::getHeight();
    point_s pp;

    int i = 0, counterPage;

    scaleAll(point, translation);
    adjastZoom(point, pos_foglio);
    data->datatouch->setPointFirstPage(translation);
    data->datatouch->zoom = 1.0;
    for(counterPage = 0; counterPage <= lenPage; counterPage ++){
        data->datatouch->newPage(n_style::white);
    }

    for(i = 0; i < point.length(); i++){
        pp = point.at(i);
        pp.m_pressure *= 0.2;
        data->datatouch->append(pp);
    }

    /*for(counterPage = 0; counterPage < lenPage; counterPage++){
        page *page;
        page = data->datatouch->at_mod(counterPage);
        for(i = 0, len = page->length(); i < len; i++){
            const int which = data->datatouch->whichPage(*page->at(i));
            if(which != page->at(i)->page){
                point_s point = *page->at(i);
                point.page = which;
                page->removeAt(i);
                data->datatouch->append(&point, which);
            }
        }
    }*/
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
    const QPointF big = bigger(point);
    const double Width = page::getWidth();
    const double delta = Width / big.x();

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
    const QPointF currentSize = bigger(point);
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

static QPointF bigger(const QList<point_s> &point)
{
    QPointF max(0.0, 0.0);
    uint i;
    const point_s *ref;

    const uint len = point.length();

    for(i = 0; i < len; i++ ){
        ref = &point.at(i);
        if(ref->m_x > max.x())
            max.setX(ref->m_x);
        if(ref->m_y > max.y())
            max.setY(ref->m_y);
    }

    return max;
}
