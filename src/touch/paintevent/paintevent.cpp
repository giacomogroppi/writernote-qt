#include "../tabletcanvas.h"
#include <QPainter>

#include "draw_image.h"
#include "../../utils/color/setcolor.h"

static bool thereispositive(datastruct *, int, int);

void TabletCanvas::paintEvent(QPaintEvent *event){
    if(!isWriting)
        isloading = true;
    isWriting = false;

    if (m_pixmap.isNull())
        initPixmap(false);

    QPainter painter;
    painter.begin(this);

    QRect pixmapPortion = QRect(event->rect().topLeft() * devicePixelRatio(),
                                    event->rect().size() * devicePixelRatio());
    painter.drawPixmap(event->rect().topLeft(), m_pixmap, pixmapPortion);

    disegnofoglio_bool += data->datatouch->posizionefoglio.isEmpty();

    if(this->m_sheet->auto_create && !disegnofoglio_bool){
        this->disegnofoglio_bool = data->datatouch->needtocreatenew();
    }

    this->disegnafoglio();

    if(this->isloading)
        load(painter);


    /*
     * la funzione disegna il rect per spostare l'oggetto
     * in caso non ci sia niente da disegnare non disegna niente
    */
    this->square_.needReload(painter);

    painter.end();
}

#define C(x) x->datatouch->m_point
#define UPDATE_LOAD(x) updateBrush_load(x->m_pressure, setcolor(&x->m_color))
#define SET_PEN(x) painter.setPen(x)

/*
 * TODO -> implement this function to play audio
*/
void TabletCanvas::load(QPainter &painter,
                        double m,
                        int size_orizzontale,
                        int size_verticale,
                        double *y_last){

    int i, len, k, _lastid;
    const point_s * __point;

    bool _need_reload = false;

    m_pixmap.fill(Qt::white);

    double xtemp[2], ytemp[2];

    if(size_orizzontale == DEFAULT_PASS_ARGUMENT_LOAD){
        size_orizzontale = width();
        size_verticale = m_pixmap.height();
    }

    QColor current_color = this->m_color;
    this->m_pen.setStyle(Qt::PenStyle::SolidLine);

    _lastid = C(data).first().idtratto;

    for(i = 1, len = C(data).length(); i < len-1; i++){
        __point = &data->datatouch->m_point.at(i);

        m_pen.setColor(setcolor(&__point->m_color));

        /*
         * la funzione trasla tutti i punti finche non ne
         * trova uno positivo, in modo da non perdere tempo
         * a ciclare
         *
         * the function translates all the points until it
         * finds a positive one, so as not to waste time cycling
        */
        _need_reload = false;
        if(C(data).at(i).m_x <= 0
                && thereispositive(data->datatouch, __point->idtratto, i)
                && __point->idtratto != IDORIZZONALE
                && __point->idtratto != IDVERTICALE){
            while(C(data).at(i).m_y <= 0){
                ++i;
            }
            _need_reload = true;
        }

        if(_need_reload)
            __point = &data->datatouch->m_point.at(i);

        if(__point->m_y < size_verticale
                && __point->m_y >= 0){

            if(!datastruct::isIdUser(__point)){
                UPDATE_LOAD(__point);

                SET_PEN(m_pen);

                for(k=0; k<2; k++){
                    /* we can draw objects which are outside the pixmap
                        qt automatically understands that you have to set negative points,
                        and those that are too high such as the margins of the pixmap
                    */

                    xtemp[k] = C(data).at(i+k).m_x;
                    ytemp[k] = C(data).at(i+k).m_y;

                }

                painter.drawLine(
                            xtemp[0], ytemp[0],
                            xtemp[1], ytemp[1]);

                __point = & data->datatouch->m_point.at(i);
                ++i;
            }
            else if(__point->m_x != size_verticale
                    && __point->idtratto == _lastid){


                UPDATE_LOAD(__point);

                SET_PEN(m_pen);

                painter.drawLine(this->lastPoint.pos*m,
                              QPointF(C(data).at(i).m_x*m, C(data).at(i).m_y*m));

            }

            lastPoint.pos.setX(__point->m_x);
            lastPoint.pos.setY(__point->m_y);

            /*
             * for pdf export
            */
            if(y_last)
                *y_last = C(data).at(i).m_y;
        }

        _lastid = __point->idtratto;

    }

    draw_image(data, painter);

    this->m_pen.setColor(current_color);

    this->isloading = false;
}

/* la funzione ritorna true se ci sono dei punti positivi per quel id tratto */
static bool thereispositive(datastruct *data, int idtratto, int start){
    int len;
    len = data->m_point.length();

    for(; start<len && data->m_point.at(start).idtratto == idtratto; start++)
        if(data->m_point.at(start).m_y >= 0.0)
            return true;

    return false;
}

/* la funzione è responsabile del settaggio dello spessore e del tipo per il load */
void TabletCanvas::updateBrush_load(float pressure, QColor color){
    /*int vValue = int(((yTilt + 60.0) / 120.0) * 255);
    int hValue = int(((xTilt + 60.0) / 120.0) * 255);*/

    /* temporary */
    int vValue = 127;
    int hValue = 127;

    switch (m_lineWidthValuator) {
            case PressureValuator:
                m_pen.setWidthF(pressureToWidth(pressure/2.00));
                break;
            case TiltValuator:
                m_pen.setWidthF(std::max(std::abs(vValue - 127),
                                         std::abs(hValue - 127)) / 12);
                break;
            default:
                m_pen.setWidthF(1);
        }
    m_brush.setColor(color);
    m_pen.setColor(color);

}
