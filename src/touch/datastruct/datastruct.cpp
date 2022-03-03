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

void datastruct::changeZoom(const double zoom, TabletCanvas *canvas)
{
    this->_zoom = zoom;
    if(canvas){
        canvas->callResizeEvent();
        canvas->_parent->zoomChange();
    }
}

void datastruct::increaseZoom(const double delta, const QSize &size)
{
    this->_zoom += delta;
    this->adjustAll(size);
}

void datastruct::drawIfInside(const QRect &area)
{
    int i = this->getFirstPageVisible();

    for(; i >= 0; i--){
        page *page = &at_mod(i);

        if(!page->isVisible())
            break;

        page->drawIfInside(-1, area);
    }
}

datastruct::datastruct(frompdf *m_pdf, fromimage *m_img)
{
    this->_pdf = m_pdf;
    this->_img = m_img;
    _last_translation = QPointF(0, 0);

    pthread_mutex_init(&_changeIdMutex, NULL);
    pthread_mutex_init(&_changeAudioMutex, NULL);
}

datastruct::~datastruct()
{
    pthread_mutex_destroy(&_changeIdMutex);
    pthread_mutex_destroy(&_changeAudioMutex);
}

void datastruct::reset(){
    this->_page.clear();
    _pointFirstPage = QPointF(0, 0);
    _zoom = 1.00;
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

void datastruct::copy(const datastruct &src, datastruct &dest)
{
    int i;
    const int len = src.lengthPage();
    int diff = len - dest.lengthPage();

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
        page::copy(src._page.at(i), dest._page.operator[](i));
    }

    dest._zoom = src._zoom;
    dest.pageVisible = src.pageVisible;
    dest._pointFirstPage = src._pointFirstPage;

    dest._last_translation = src._last_translation;
}
