#include "page.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"
#include "../tabletcanvas.h"
#include <QPainter>
#include "../../utils/time/current_time.h"
#include <QPainterPath>
#include "../../utils/common_error_definition.h"
#include "../../datawrite/source_read_ext.h"
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
    this->mergeList();
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
        style.colore.fromColor(TEMP_COLOR);
        style.thickness =  widthToPressure(TEMP_TICK);
    }

    newStrokeVertical.setMetadata(this->count, IDVERTICALE, -1, style.colore);
    newStrokeOrizzontal.setMetadata(this->count, IDORIZZONALE, -1, style.colore);

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
        this->m_stroke_writernote.append(newStrokeOrizzontal);
    if(newStrokeVertical.length())
        this->m_stroke_writernote.append(newStrokeVertical);

}

void page::drawEngine(QPainter &painter, QList<stroke> &List,
                      const int m_pos_ris)
{
    struct Point lastPoint;
    int i;
    QBrush m_brush;
    QPen m_pen(m_brush, 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);
    QPointF pointDraw;
    int lenStroke = List.length();

    for(i = 0; i < lenStroke; i++){
        const stroke &stroke = List.at(i);
        if(stroke.isEmpty()){
            List.removeAt(i);
            i --;
            lenStroke --;
            continue;
        }

        const int decrease = (stroke.getPosizioneAudio() > m_pos_ris) ? 1 : 4;
        const float pressure = TabletCanvas::pressureToWidth(stroke.getPressure() / 2.00) * PROP_RESOLUTION;
        m_pen.setColor(stroke.getColor(decrease));

        if(stroke.constantPressure()){
            const QPainterPath &path = stroke.getQPainterPath();

            m_pen.setWidthF(pressure);

            painter.strokePath(path, m_pen);

        }else{
            int counterPoint;
            const int lenPoint = stroke.length();
            const int refCounter = this->count - 1;

            lastPoint.pos = at_translation(stroke.at(0), refCounter).toQPointF(PROP_RESOLUTION);

            for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
                const point_s point = at_translation(stroke.at(counterPoint), refCounter);
                pointDraw = point.toQPointF(PROP_RESOLUTION);

                m_pen.setWidthF(TabletCanvas::pressureToWidth(point.pressure / 2.00) * PROP_RESOLUTION);

                painter.setPen(m_pen);

                painter.drawLine(lastPoint.pos, pointDraw);

                lastPoint.pos = pointDraw;
            }
        }
    }

}

inline void page::draw(QPainter &painter, const int m_pos_ris, const bool all)
{
    painter.setRenderHint(QPainter::TextAntialiasing, false);

    if(all)
        this->drawEngine(painter, this->m_stroke, m_pos_ris);

    this->drawEngine(painter, this->strokeTmp, m_pos_ris);

    this->mergeList();
}

inline void page::mergeList()
{
    int i;
    const int len = this->strokeTmp.length();
    int index = m_stroke.length();

    //if(len)
    //   qDebug() << "page::mergeList" << len;


    for(i = 0; i < len; i++){
        const stroke &stroke = strokeTmp.at(i);

        m_stroke.append(stroke);

        Q_ASSERT(m_stroke.at(index).length() == strokeTmp.at(i).length());
        Q_ASSERT(m_stroke.at(index).getId() == strokeTmp.at(i).getId());

        index ++;
    }

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
        style.colore.fromColor(Qt::black);

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

    for(i = 0; i< (uint)style.ny; i++){
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
    return this->m_stroke.length();
}

/*
 * all --> indicates if all the points must be drawn from scratch, if false it is drawn over the old image
*/
void page::triggerRenderImage(int m_pos_ris, const bool all)
{
    const bool isNull = imgDraw.isNull();

    if(all || isNull)
        this->imgDraw = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);

    /* testing */
    //imgDraw.fill(Qt::white);

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
        this->m_stroke.append(stroke());
    }
}

#define DO_CTRL(function) \
    err = function; \
    if(err != OK) \
        return err;

int page::save(zip_source_t *file) const
{
    int i, err = OK;
    int len = lengthStroke();

    for(i = 0; i < len; i++){
        DO_CTRL(atStroke(i).save(file));
    }

    len = this->m_stroke_writernote.length();
    SOURCE_WRITE_RETURN(file, &len, sizeof(len));

    for(i = 0; i < len; i++){
        DO_CTRL(m_stroke_writernote.at(i).save(file));
    }

    return OK;
}

#define DO_LOAD(list) \
    for(i = 0; i < len_stroke; i++){ \
        stroke tmp; \
        DO_CTRL(tmp.load(file, ver_stroke)); \
        this->list.append(tmp); \
    }

int page::load(zip_file_t *file, int ver_stroke, int len_stroke)
{
    int i, err;

    DO_LOAD(m_stroke);

    SOURCE_READ_RETURN(file, &len_stroke, sizeof(int));

    DO_LOAD(m_stroke_writernote);

    return OK;
}

