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
static void drawLineOrizzontal(stroke &stroke, point_s &point, const style_struct_S &style, const double &last,
                            double &deltax, const double &width_p, const double &ct_del);
static void drawLineVertical(stroke &stroke, point_s &point, const style_struct_S &style,
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

bool page::needtochangeid(const int IndexStroke, const int indexInStroke) const
{
    const stroke &stroke = atStroke(IndexStroke);

    const int lenPointStroke = stroke.length() - 1;

    return indexInStroke == (lenPointStroke - 1) || (lenPointStroke + 1);
}

void page::drawNewPage(n_style __style)
{
    bool fast = false;
    double deltax, deltay, ct_del;
    struct style_struct_S style;
    struct point_s tmp_point;
    stroke newStrokeVertical, newStrokeOrizzontal;
    const double width_p    = this->getWidth();
    const double height_p   = this->getHeight();
    const double last = (count-1)*page::getHeight();

    setStylePrivate(fast, __style, style);

    if(fast){
        setcolor_struct(&style.colore, TEMP_COLOR);
        style.thickness =  widthToPressure(TEMP_TICK);
    }

    newStrokeVertical.setMetadata(this->count, IDVERTICALE, -1, style.colore);

    if(style.nx){
        deltax = height_p / (double)style.nx;
        ct_del = deltax;
    }
    if(style.ny)
        deltay = width_p / (double)style.ny;

    drawLineOrizzontal(newStrokeOrizzontal, tmp_point, style, last, deltax, width_p, ct_del);
    drawLineVertical(newStrokeVertical, tmp_point, style, last, deltay, height_p);

    newStrokeOrizzontal.__setPressureForAllPoint(widthToPressure(style.thickness));
    newStrokeVertical.__setPressureForAllPoint(widthToPressure(style.thickness));

    if(newStrokeOrizzontal.length())
        this->append(newStrokeOrizzontal);
    if(newStrokeVertical.length())
        this->append(newStrokeVertical);

}

void page::drawEngine(QPainter &painter, QList<stroke> &List, int i,
                      const int m_pos_ris)
{
    //int _lastid = List.at(i).idtratto;
    //int _lastid = IDUNKNOWN;

    //const int page = this->count-1;
    //const int len = List.length();
    struct Point lastPoint;
    //point_s *point, *point1, *point2;
    //static const double delta = PROP_RESOLUTION;

    QPainterPath path;
    QBrush m_brush;
    QPen m_pen(m_brush, 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);

    //static double x, y;
    static QPointF pointDraw;

    m_pen.setStyle(Qt::PenStyle::SolidLine);
    //qDebug() << painter.renderHints();

    int lenStroke = lengthStroke();

    for(i = 0; i < lenStroke; i++){
        const stroke &stroke = List.at(i);
        const int decrease = (stroke.getPosizioneAudio() > m_pos_ris) ? 1 : 4;
        m_pen.setColor(stroke.getColor(decrease));

        if(!stroke.isIdUser())
            continue;
        if(!stroke.constantPressure()){
            int counterPoint, lenPoint;

            lastPoint.pos = QPointF(stroke.at(0).m_x, stroke.at(0).m_y);

            for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
                const point_s &point = stroke.at(counterPoint);
                m_pen.setWidthF(TabletCanvas::pressureToWidth(point.pressure/2.00) * PROP_RESOLUTION);

                painter.setPen(m_pen);

                painter.drawLine(lastPoint.pos, pointDraw);
            }
        }else{
            const QPainterPath &path = stroke.getQPainterPath();

            painter.strokePath(path, m_pen);
        }
    }

    /*for(; i < len-3; ++i){
        path.moveTo(List.at(i).toQPointF(delta));

        while(i<len-3 && List.at(i+2).idtratto == _lastid){
            point = at_translation(List, i);
            point1 = at_translation(List, i+1);
            point2 = at_translation(List, i+2);

            if(point->page != page || point1->page != page || point2->page != page)
                break;

            const int decrease = (point->m_posizioneaudio > m_pos_ris) ? 1 : 4;

            m_pen.setColor(setcolor(point->m_color));
            TabletCanvas::updateBrush_load(point->m_pressure*delta,
                                               setcolor(&point->m_color, decrease),
                                               m_pen, m_brush);
            m_pen.setWidthF(point->m_pressure*10*delta);
            painter.setPen(m_pen);

            path.cubicTo(point->toQPointF(delta), point1->toQPointF(delta), point2->toQPointF(delta));

            i += 2;
        }

        painter.strokePath(path, m_pen);

        nextPoint(i, List);
        if(i < len-3)
            _lastid = at(i)->idtratto;
    }*/

    /*for(; i < len; ++i){
        point = at_translation(List, i);

        pointDraw = QPointF(point->m_x, point->m_y) * PROP_RESOLUTION;

        if(point->idtratto == _lastid && point->page == page){
            const int decrease = (point->m_posizioneaudio > m_pos_ris) ? 1 : 4;
            //point->m_pressure *= 1.4;

            //m_pen.setColor(setcolor(point->m_color, decrease));
            TabletCanvas::updateBrush_load(point->m_pressure,
                setcolor(&point->m_color, decrease),
                m_pen, m_brush);

            m_pen.setWidthF(m_pen.widthF() * PROP_RESOLUTION);

            painter.setPen(m_pen);

            painter.drawLine(lastPoint.pos, pointDraw);
        }

        lastPoint.pos = pointDraw;

        _lastid = point->idtratto;
    }*/

}

inline void page::draw(QPainter &painter, const int m_pos_ris, const bool all)
{
    int i = 0;
    int len = lengthStroke();


    painter.setRenderHint(QPainter::TextAntialiasing, false);

    if(len)
        this->moveToUserPoint(i);

    if(all)
        this->drawEngine(painter, this->m_stroke, i, m_pos_ris);

    i=0;
    this->drawEngine(painter, this->strokeTmp, i, m_pos_ris);

    this->mergeList();
}

inline void page::mergeList()
{
    const int len = this->strokeTmp.length();
    LOG_CONDITION(len > 1, "void page::mergeList", log_ui::type_write::possible_bug);

#ifdef DEBUGINFO
    if(len > 1)
        std::abort();
#endif
    m_stroke.append(this->strokeTmp.at(0));
    strokeTmp.clear();
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

        style.nx = TEMP_SQUARE*(page::getHeight() / page::getWidth());
        style.ny = TEMP_SQUARE;
    }
}

static inline double widthToPressure(double v){
    return v/10.0;
}

static void drawLineOrizzontal(stroke &stroke, point_s &point, const style_struct_S &style,
                            const double &last, double &deltax, const double &width_p, const double &ct_del){
    uint i;

    for(i=0; i< (uint)style.nx; ++i){
        point.m_x = 0;
        point.m_y = last + deltax;

        stroke.append(point);

        point.m_x = width_p;
        stroke.append(point);

        deltax += ct_del;
    }
}

static void drawLineVertical(stroke &stroke, point_s &point, const style_struct_S &style,
                            const double &last, double &deltay, const double &height_p){
    if(!style.ny)
        return;

    const double ct_del = deltay;
    uint i;

    for(i=0; i< (uint)style.ny; i++){
        point.m_x = deltay;
        point.m_y = last; /* corrisponde to 0 */

        stroke.append(point);

        point.m_y = height_p + last;
        stroke.append(point);

        deltay += ct_del;

    }
}

bool page::userWrittenSomething() const
{
    uint i;
    const uint len = lengthStroke();
    for(i=0; i<len && !atStroke(i).isIdUser(); i++);

    //qDebug() << QString("Page %1 %2").arg(this->count-1).arg(!i || !(i==len));
    return !(i==len);
}

/*
 * all --> indicates if all the points must be drawn from scratch, if false it is drawn over the old image
*/
void page::triggerRenderImage(int m_pos_ris, const bool all)
{
    const bool isNull = imgDraw.isNull();

    if(all || isNull)
        this->imgDraw = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);

    QPainter painter;
    painter.begin(&imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    this->draw(painter, m_pos_ris, (all || isNull));

    painter.end();

    return;
    if(!imgDraw.save("/home/giacomo/Scrivania/tmp_foto/foto"+current_time_string()+".png", "PNG", 0))
        std::abort();

}

void page::allocateStroke(int numAllocation)
{
    for(int i = 0; i < numAllocation; i++){
        stroke stroke;
        this->append(stroke);
    }
}
