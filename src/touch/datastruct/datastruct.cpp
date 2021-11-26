#include "datastruct.h"
#include <QList>
#include <QDebug>
#include "../../sheet/fast-sheet/fast_sheet_ui.h"
#include "../tabletcanvas.h"
#include "../../mainwindow.h"

#define mov_if_neg(p, x) \
    p = x; \
    return;

void datastruct::changeZoom(const double zoom, /*TabletCanvas*/ TabletCanvas *canvas)
{
    this->zoom = zoom;
    if(canvas){
        canvas->callResizeEvent();
        canvas->parent->zoomChange();
    }
}

void datastruct::increaseZoom(const double delta, const QSize &size)
{
    this->zoom += delta;
    this->adjustAll(size);
}

datastruct::datastruct(frompdf *m_pdf, fromimage *m_img)
{
    this->m_pdf = m_pdf;
    this->m_img = m_img;
    __last_translation = QPointF(0, 0);
}

/* the function returns true if the id is available */
bool datastruct::isAvailable(int id) const
{
    uint i, len, counterPage;
    const page *page;
    const uint lenPage = this->lengthPage();

    for(counterPage = 0; counterPage < lenPage; counterPage ++){
        page = &this->at(counterPage);
        len = page->lengthStroke();
        for(i=0; i<len; i++){
            if(page->atStroke(i).getId() == id)
                return 0;
        }
    }

    return 1;
}

void datastruct::reset(){
    this->m_page.clear();
    pointFirstPage = QPointF(0, 0);
    zoom = 1.00;
}

//{page; indexInPage}
std::tuple<uint, uint> datastruct::decreaseAlfa(const int id,
                              const uchar decrease,
                              const int lenPage)
{
    int i, counterPage, len;
    page *page_mod;
    const page *page_read;
    uint lastI, lastCounter;

    for(counterPage = 0; counterPage < lenPage; counterPage++){
        page_read = &at(counterPage);
        len = page_read->lengthStroke();

        page_read->moveToUserPoint(i);

        if(i < len)
            page_mod = &at_mod(counterPage);

        for(; i < len; ++i){
            stroke &stroke = page_mod->atStrokeMod(i);

            if(stroke.getId() == id){
                const uchar newAlfa = stroke.getColor().alpha() / decrease;
                stroke.setAlfaColor(newAlfa);

                lastI = i;
                lastCounter = counterPage;
            }
        }
    }

    return {lastCounter, lastI};
}

void datastruct::copy(const datastruct &src, datastruct &dest)
{
    uint i;
    const uint len = src.lengthPage();
    uint diff = len - dest.lengthPage();

    if(diff > 0){
        for(i = 0; i < diff; i++){
            dest.newPage(n_style::white);
        }
    }
    else if(diff != 0){
        diff = -diff;
        for(i = 0; i < diff; i++)
            dest.removePage(i);
    }

    for(i=0; i<len; ++i){
        page::copy(src.m_page.at(i), dest.m_page.operator[](i));
    }

    dest.zoom = src.zoom;

    dest.__last_translation = src.__last_translation;
}

void datastruct::removeat(const uint index, const uint page){
    this->m_page.operator[](page).removeAt(index);
}

/*
 * the function return true if the point in position i
 * is in the middle of a stretch
 *
 * return false if it's the first point or the lastone
 */
bool datastruct::needtochangeid(const uint index, const uint indexPage){
    const page *page;
    const uint lenPointForPage = at(indexPage)->length();

    if(!index || (index+1) >= lenPointForPage)
        return false;

    page = at(indexPage);
    int temp_id = page->at(index)->idtratto;

    return (temp_id == page->at(index-1)->idtratto
            && temp_id == page->at(index+1)->idtratto);

}
