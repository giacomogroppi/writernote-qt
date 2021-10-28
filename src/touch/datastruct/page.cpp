#include "page.h"
#include "../../utils/color/setcolor.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"
#include "../tabletcanvas.h"
#include <QPainter>

#define UPDATE_LOAD(x, div, m_pen, m_brush ) \
        TabletCanvas::updateBrush_load(x.m_pressure, setcolor(&x.m_color, div), TabletCanvas::Valuator::PressureValuator, m_pen, m_brush);

static inline double widthToPressure(double v);
static void setStylePrivate(bool &fast, n_style res, style_struct_S &style);
static void drawLineOrizzontal(QList<point_s> &list, point_s &point, const style_struct_S &style, const double &last,
                            double &deltax, const double &width_p, const double &ct_del);
static void drawLineVertical(QList<point_s> &list, point_s &point, const style_struct_S &style,
                            const double &last, double &deltay, const double &height_p);


#define TEMP_COLOR Qt::black
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

page::page(const int count, const n_style style)
{
    this->count = count;
    this->IsVisible = true;
    drawNewPage(style);
}

bool page::needtochangeid(const uint index) const
{
    const uint len = length();
    const point_s *point;

    if(index == (len-1) || !index)
        return true;

    point = at(index);
    if(point->idtratto == at(index+1)->idtratto || point->idtratto == at(index-1)->idtratto)
        return false;
    return true;

}

void page::drawNewPage(n_style __style)
{
    bool fast = false;
    double deltax, deltay, ct_del;
    struct style_struct_S style;
    struct point_s tmp_point;
    const double width_p    = this->getWidth();
    const double height_p   = this->getHeight();
    const double last = (count-1)*page::getHeight();

    setStylePrivate(fast, __style, style);

    if(fast){
        setcolor_struct(&style.colore, TEMP_COLOR);
        style.thickness =  widthToPressure(TEMP_TICK);
    }

    memcpy(&tmp_point.m_color, &style.colore, sizeof(style.colore));

    /*style.nx = (style.nx <= 0) ? 1 : style.nx;
    style.ny = (style.ny <= 0) ? 1 : style.ny;*/

    if(style.nx){
        deltax = height_p / (double)style.nx;
        ct_del = deltax;
    }
    if(style.ny)
        deltay = width_p / (double)style.ny;


    tmp_point.m_pressure = widthToPressure(style.thickness);

    //qDebug() << "page::drawNewPage " << last << width_p << height_p << style.ny << style.nx << deltay << (__style==n_style::square);

    /* draw the orizzontal line */
    drawLineOrizzontal(this->m_point, tmp_point, style, last, deltax, width_p, ct_del);
    /* draw vertical line */
    drawLineVertical(this->m_point, tmp_point, style, last, deltay, height_p);
}

inline void page::draw(QPainter &painter, const int m_pos_ris, const bool is_play)
{
    int i;
    const int len = length();
    QPen m_pen;
    QBrush m_brush;
    int _lastid = IDUNKNOWN;
    struct Point lastPoint;

    for(i=0; i<len-1; ++i){
        if(at(i)->isIdUser())
            break;
    }

    for(; i < len-1; ++i){
        const auto &point = *at(i);
        //at_draw(i, QPointF(0, 0), point, zoom);
        m_pen.setColor(setcolor(point.m_color));

        if(!point.isIdUser())
                continue;

            if(point.idtratto == _lastid){
                if(is_play && point.m_posizioneaudio > m_pos_ris)
                {
                    UPDATE_LOAD(point, 4, m_pen, m_brush);
                }
                else
                {
                    UPDATE_LOAD(point, 1, m_pen, m_brush);
                }

                painter.setPen(m_pen);

                painter.drawLine(lastPoint.pos,
                    QPointF(point.m_x, point.m_y));

            }

            lastPoint.pos.setX(point.m_x);
            lastPoint.pos.setY(point.m_y);

            _lastid = point.idtratto;
        }
}

static void setStylePrivate(bool &fast, n_style res, style_struct_S &style){
    if(res == n_style::empty){
        res = n_style::square;
    }

    if(res == n_style::line){
        fast = true;

        style.nx = TEMP_N_X;
        style.ny = 0;
    }
    else if(res == n_style::white){
        /* we set the color manually */
        setcolor_struct(&style.colore, Qt::black);

        style.nx = 0;
        style.ny = 0;
    }else{
        fast = true;

        style.nx = TEMP_SQUARE;
        style.ny = TEMP_SQUARE;
    }
}

static inline double widthToPressure(double v){
    return v/10.0;
}

static void drawLineOrizzontal(QList<point_s> &list, point_s &point, const style_struct_S &style,
                            const double &last, double &deltax, const double &width_p, const double &ct_del){
    uint i;
    point.idtratto = IDORIZZONALE;

    for(i=0; i< (uint)style.nx; ++i){
        point.m_x = 0;
        point.m_y = last + deltax;

        list.append(point);

        point.m_x = width_p;
        list.append(point);

        deltax += ct_del;
    }
}

static void drawLineVertical(QList<point_s> &list, point_s &point, const style_struct_S &style,
                            const double &last, double &deltay, const double &height_p){
    if(!style.ny)
        return;

    const double ct_del = deltay;
    uint i;

    point.idtratto = IDVERTICALE;

    for(i=0; i< (uint)style.ny; i++){
        point.m_x = deltay;
        point.m_y = last; /* corrisponde to 0 */

        list.append(point);

        point.m_y = height_p + last;
        list.append(point);

        deltay += ct_del;

    }
}

bool page::userWrittenSomething() const
{
    uint i;
    const uint len = length();
    for(i=0; i<len && !at(i)->isIdUser(); i++);

    return !(i==len);
}

void page::triggerRenderImage(int m_pos_ris, const bool is_play)
{
    /* we need Format_RGB888 for 255-255-255 color */
    this->imgDraw = QImage(page::getWidth(), page::getHeight(), QImage::Format_RGB888);
    QPainter painter;
    painter.begin(&imgDraw);

    this->draw(painter, m_pos_ris, is_play);

    painter.end();
}
