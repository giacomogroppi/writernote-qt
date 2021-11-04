#include "page.h"
#include "../../utils/color/setcolor.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"
#include "../tabletcanvas.h"
#include <QPainter>
#include "../../utils/time/current_time.h"
#include <QPainterPath>
#define UPDATE_LOAD(x, divColor, m_pen, m_brush ) \


static inline double widthToPressure(double v);
static void setStylePrivate(bool &fast, n_style res, style_struct_S &style);
static void drawLineOrizzontal(QList<point_s> &list, point_s &point, const style_struct_S &style, const double &last,
                            double &deltax, const double &width_p, const double &ct_del);
static void drawLineVertical(QList<point_s> &list, point_s &point, const style_struct_S &style,
                            const double &last, double &deltay, const double &height_p);


#define TEMP_COLOR Qt::black
#define TEMP_TICK 10
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

    if(style.nx){
        deltax = height_p / (double)style.nx;
        ct_del = deltax;
    }
    if(style.ny)
        deltay = width_p / (double)style.ny;

    tmp_point.m_pressure = widthToPressure(style.thickness);

    drawLineOrizzontal(this->m_point, tmp_point, style, last, deltax, width_p, ct_del);
    drawLineVertical(this->m_point, tmp_point, style, last, deltay, height_p);
}

void page::drawEngine(QPainter &painter, QList<point_s> &List, int i, const bool is_play,
                      const int m_pos_ris)
{
    int _lastid = IDUNKNOWN;
    const int page = this->count-1;
    const int len = List.length();
    struct Point lastPoint;
    point_s *point;
    static const double delta = getResolutionWidth() / getWidth();

    QPainterPath path;
    QBrush m_brush;
    QPen m_pen(m_brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    static double x, y;

    m_pen.setStyle(Qt::PenStyle::SolidLine);
    qDebug() << painter.renderHints();
    for(; i < len-3; ++i){
        point = at_translation(List, i);

        x = point->m_x * delta;
        y = point->m_y * delta;

        if(point->idtratto == _lastid && point->page == page){
            const int decrease = (is_play && point->m_posizioneaudio > m_pos_ris) ? 4 : 1;
            point->m_pressure *= 1.32;

            m_pen.setColor(setcolor(point->m_color));
            TabletCanvas::updateBrush_load(point->m_pressure*delta,
                                           setcolor(&point->m_color, decrease),
                                           m_pen, m_brush);
            m_pen.setWidthF(20);
            painter.setPen(m_pen);

            painter.drawLine(lastPoint.pos, QPointF(x, y));
        }

        lastPoint.pos = QPointF(x, y);

        _lastid = point->idtratto;

        i += 2;
    }
}

inline void page::draw(QPainter &painter, const int m_pos_ris, const bool is_play, const bool all)
{
    int i;
    int len = length();

    if(!len)
        return;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, false);

    this->moveToUserPoint(i);

    if(all)
        this->drawEngine(painter, this->m_point, i, is_play, m_pos_ris);

    i=0;
    this->drawEngine(painter, this->tmp, i, is_play, m_pos_ris);

    this->mergeList();
}

void page::mergeList(){
    int i, len;
    len = this->tmp.length();
    for(i = 0; i < len; i++){
        this->m_point.append(tmp.at(i));
    }
    tmp.clear();
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

/*
 * all --> indicates if all the points must be drawn from scratch, if false it is drawn over the old image
*/
void page::triggerRenderImage(int m_pos_ris, const bool is_play, const bool all)
{
    if(all || this->imgDraw.isNull())
        this->imgDraw = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&imgDraw);

    this->draw(painter, m_pos_ris, is_play, all);

    painter.end();

    //return;
    if(!imgDraw.save("/home/giacomo/Scrivania/tmp_foto/foto"+current_time_string()+".png", "PNG", 1))
        std::abort();
}
