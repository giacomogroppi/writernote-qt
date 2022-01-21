#include "datastruct.h"
#include <QList>
#include <QDebug>
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "touch/tabletcanvas.h"
#include "mainwindow.h"

#define mov_if_neg(p, x) \
    p = x; \
    return;

void datastruct::newPage(int num)
{
    while (num > 0){
        this->newPage(n_style::white);
        num --;
    }
}

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
    pthread_mutex_init(&changeIdMutex, NULL);
}

/* the function returns true if the id is available */
bool datastruct::isAvailable(int id) const
{
    int len;
    const page *page;
    int lenPage = this->lengthPage() - 1;

    for(; lenPage >= 0; lenPage --){
        page = &at(lenPage);
        len = page->lengthStroke() - 1;
        for(; len >= 0; len --){
            if(page->atStroke(len).getId() == id)
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

void datastruct::decreaseAlfa(const QVector<int> &pos, int index)
{
    at_mod(index).decreseAlfa(pos, 4);
}

// this function is not thread safe
void datastruct::decreaseAlfa(stroke &stroke, page &page, cint decrease)
{
    const uchar newAlfa = stroke.getColor().alpha() / decrease;
    stroke.setAlfaColor(newAlfa);

    /*
    * before drawing the new stroke with
    * the new color we need to set all
    * the area it previously occupied to white
    */

    page.drawForceColorStroke(stroke, -1, COLOR_NULL, NULL);
    page.drawStroke(stroke, -1);
}

//{page; indexInPage}
void datastruct::decreaseAlfa(cint id,
                              cuchar decrease)
{
    int counterPage, strokeCounter;
    page *page_mod;
    stroke *stroke;
    const page *page_read;

    counterPage = this->lengthPage() - 1;

    for(; counterPage >= 0; counterPage--){
        page_read = &at(counterPage);

        strokeCounter = page_read->lengthStroke() - 1;
        if(!strokeCounter) continue;

        page_mod = &at_mod(counterPage);

        for(strokeCounter = 0; strokeCounter >= 0; strokeCounter--){
            stroke = &page_mod->atStrokeMod(strokeCounter);

            if(stroke->getId() == id){
                this->decreaseAlfa(*stroke, *page_mod, decrease);

                /* since there cannot be another trait that has the same
                 * index, we can directly return to the caller */
                return;
            }
        }
    }
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
    dest.pageVisible = src.pageVisible;
    dest.pointFirstPage = src.pointFirstPage;

    dest.__last_translation = src.__last_translation;
}
