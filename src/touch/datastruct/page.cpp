#include "page.h"
#include "sheet/fast-sheet/fast_sheet_ui.h"
#include "../tabletcanvas.h"
#include <QPainter>
#include "utils/time/current_time.h"
#include <QPainterPath>
#include "utils/common_error_definition.h"
#include "datawrite/source_read_ext.h"
#include "time.h"
#include "utils/common_script.h"

static inline double widthToPressure(double v);
static void setStylePrivate(bool &fast, n_style res, style_struct_S &style);
static void drawLineOrizzontal(stroke &stroke, point_s &point, const style_struct_S &style, const double &last,
                            double &deltax, const double &width_p, const double &ct_del);
static void drawLineVertical(stroke &stroke, point_s &point, const style_struct_S &style,
                            const double &last, double &deltay, const double &height_p);

#define Define_PEN(pen) QPen pen(QBrush(), 1.0, Qt::SolidLine, Qt::MPenCapStyle, Qt::RoundJoin);

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

void page::drawStroke(
        QPainter        &painter,
        const stroke    &stroke,
        QPen            &m_pen,
        const QColor    &color) const
{
    QPointF lastPoint, pointDraw;

    m_pen.setColor(color);
    m_pen.setWidthF(TabletCanvas::pressureToWidth(stroke.getPressure() / 2.00) * PROP_RESOLUTION);

    if(color == COLOR_NULL){
        m_pen.setWidthF(m_pen.widthF() * 1.5);
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
    }

    if(stroke.constantPressure()){
        const QPainterPath &path = stroke.getQPainterPath();

        painter.strokePath(path, m_pen);

    }else{
        int counterPoint;
        const int lenPoint = stroke.length();
        const int refCounter = this->count - 1;

        lastPoint = at_translation(stroke.at(0), refCounter).toQPointF(PROP_RESOLUTION);

        for(counterPoint = 1; counterPoint < lenPoint; counterPoint ++){
            const point_s point = at_translation(stroke.at(counterPoint), refCounter);
            pointDraw = point.toQPointF(PROP_RESOLUTION);

            m_pen.setWidthF(TabletCanvas::pressureToWidth(point.pressure / 2.00) * PROP_RESOLUTION);

            painter.setPen(m_pen);

            painter.drawLine(lastPoint, pointDraw);

            lastPoint = pointDraw;
        }
    }
}

void page::drawEngine(
        QPainter        &painter,
        QList<stroke>   &List,
        int       m_pos_ris)
{
    int i;
    Define_PEN(m_pen);
    int lenStroke = List.length();

    //auto __clock = clock();

    for(i = 0; i < lenStroke; i++){
        const stroke &stroke = List.at(i);
        if(stroke.isEmpty()){
            List.removeAt(i);
            i --;
            lenStroke --;
            log_write->write("Stroke is empty", log_ui::type_write::possible_bug);
            continue;
        }

        const QColor color = stroke.getColor(
            (stroke.getPosizioneAudio() > m_pos_ris) ? 1 : 4
        );

        this->drawStroke(painter, stroke, m_pen, color);
    }

}

inline void page::draw(QPainter &painter, int m_pos_ris, bool all)
{

    if(all)
        this->drawEngine(painter, this->m_stroke, m_pos_ris);

    this->drawEngine(painter, this->strokeTmp, m_pos_ris);

    this->mergeList();
}

void page::mergeList()
{
    int i;
    const int len = this->strokeTmp.length();
    int index = m_stroke.length();

    for(i = 0; i < len; i++){
        const stroke &stroke = strokeTmp.at(i);

        m_stroke.append(stroke);

        Q_ASSERT(m_stroke.at(index).length() == strokeTmp.at(i).length());
        Q_ASSERT(m_stroke.at(index).getId() == strokeTmp.at(i).getId());

        index ++;
    }

    strokeTmp.clear();
}

static void setStylePrivate(
    bool            &fast, 
    n_style         res, 
    style_struct_S  &style)
{
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

static Q_ALWAYS_INLINE void drawLineOrizzontal(
    stroke                  &stroke, 
    point_s                 &point, 
    const style_struct_S    &style,
    const double            &last, 
    double                  &deltax, 
    const double            &width_p, 
    const double            &ct_del)
{
    uint i;

    for(i = 0; i < (uint)style.nx; i++){
        point.m_x = 20;
        point.m_y = last + deltax - 20;

        stroke.append(point);

        point.m_x = width_p - 20;
        stroke.append(point);

        deltax += ct_del;
    }
}

static Q_ALWAYS_INLINE void drawLineVertical(
    stroke                  &stroke, 
    point_s                 &point, 
    const style_struct_S    &style, 
    const double            &last, 
    double                  &deltay, 
    const double            &height_p)
{    
    const double ct_del = deltay;
    uint i;

    for(i = 0; i < (uint)style.ny; i++){
        point.m_x = deltay  - 20;
        point.m_y = last    + 20; /* corrisponde to 0 */

        stroke.append(point);

        point.m_y = height_p + last - 20;
        stroke.append(point);

        deltay += ct_del;

    }
}

static Q_ALWAYS_INLINE void __initImg(QImage &img)
{
    img = QImage(page::getResolutionWidth(), page::getResolutionHeigth(), QImage::Format_ARGB32);
}

void page::drawToImage(
    const QVector<int>  &index, 
    QImage              &img,
    cint                flag) const
{
    QPainter painter;
    Define_PEN(pen);

    if(flag & DR_IMG_INIT_IMG){
        __initImg(img);
    }
    
    painter.begin(&img);

    for (const auto &__index : index){
        const stroke &stroke = atStroke(__index);
        this->drawStroke(painter, stroke, pen, stroke.getColor());
    }

    painter.end();
}

bool page::userWrittenSomething() const
{
    return this->m_stroke.length();
}

bool page::initImg(bool flag)
{
    flag = imgDraw.isNull() || flag;
    if(flag)
        __initImg(imgDraw);

    return flag;
}

/*
 * all --> indicates if all the points must be drawn from scratch, if false it is drawn over the old image
*/
void page::triggerRenderImage(int m_pos_ris, bool all)
{
    all = initImg(all);

    /* testing */
    //imgDraw.fill(Qt::white);

    QPainter painter;
    painter.begin(&imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    this->draw(painter, m_pos_ris, all);

    painter.end();

    return;
    if(!imgDraw.save("~/Scrivania/tmp_foto/foto"+current_time_string()+".png", "PNG", 0))
        std::abort();

}

#define MakeCTRL(point) \
    if(datastruct::isinside(topLeft1, bottomRight1, point)) \
        return true;

static Q_ALWAYS_INLINE int __is_inside_suare(
        const QRectF    &rect1,
        const QRectF    &rect2)
{
    const QPointF &topLeft1     = rect1.topLeft();
    const QPointF &bottomRight1 = rect1.bottomRight();

    MakeCTRL(rect2.bottomRight());
    MakeCTRL(rect2.bottomLeft());

    MakeCTRL(rect2.topRight());
    MakeCTRL(rect2.topLeft());

    return false;
}

static Q_ALWAYS_INLINE int is_inside_squade(
        const QRectF    &rect1,
        const QRectF    &rect2)
{
    return __is_inside_suare(rect1, rect2) || __is_inside_suare(rect2, rect1);
}

// la funzione identifica i punti con id QList<int> &id
// disegna lo stroke con colore bianco, e identifica
// automaticamente gli stroke che vanno ridisegnati
// perchè potrebbero esser stati compromessi nell'immagine
// della classe page
// Ritorna 1 se abbiamo ridisegnato
//
// Si suppone che l'area da ridisegnare sia solo in questa pagina
int page::removeAndDraw(
        int                 m_pos_ris,
        const QList<int>    &id,
        const QRectF        &area)
{
    int index = lengthStroke() - 1;
    int mod = 0;

    for(; index >= 0; index --){
        const stroke &stroke = atStroke(index);
        if(IS_PRESENT_IN_LIST(id, stroke.getId())){
            this->drawForceColorStroke(stroke, m_pos_ris, COLOR_NULL);
            mod = 1;
            removeAt(index);
        }
    }

    index = this->lengthStroke() - 1;
    for(; index >= 0; index --){
        const stroke &stroke = this->atStroke(index);
        if(is_inside_squade(stroke.getBiggerPointInStroke(), area)){
            this->drawStroke(stroke, m_pos_ris);
        }
    }

    return mod;
}

void page::drawForceColorStroke(const stroke &stroke, int m_pos_ris, const QColor &color)
{
    QPainter painter;
    Define_PEN(pen);

    if(initImg(false))
        return this->triggerRenderImage(m_pos_ris, true);

    painter.begin(&this->imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    this->drawStroke(painter, stroke, pen, color);

    painter.end();
}

// the function is use for rubber and square
void page::drawForceColor(int m_pos_ris, const QList<int> &id, const QColor &color)
{
    QPainter painter;
    Define_PEN(pen);

    if(initImg(false))
        return this->triggerRenderImage(m_pos_ris, true);

    painter.begin(&this->imgDraw);
    painter.setRenderHint(QPainter::Antialiasing, true);

    for(const stroke &stroke: qAsConst(m_stroke)){
        if(IS_PRESENT_IN_LIST(id, stroke.getId())){
            this->drawStroke(painter, stroke, pen, color);
        }
    }

    painter.end();
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

void page::drawStroke(const stroke &stroke, int m_pos_ris)
{
    drawForceColorStroke(stroke, m_pos_ris, stroke.getColor(1.0));
}
