#include "page.h"
#include "../../utils/color/setcolor.h"
#include "../../sheet/fast-sheet/fast_sheet_ui.h"

static inline double widthToPressure(double v);
static void setStylePrivate(bool &fast, n_style res, style_struct_S &style);
static void drawLineOrizzontal(QList<point_s> &list, point_s &point, style_struct_S &style, const double &last,
                            double &deltax, const double &width_p, const double &ct_del);
static void drawLineVertical(QList<point_s> &list, point_s &point, style_struct_S &style,
                            const double &last, double &deltay, const double &height_p);


#define TEMP_COLOR Qt::black
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

page::page(const int count, const n_style style)
{
    this->count = count;
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
    const double width_p    = this->currentWidth();
    const double height_p   = this->currentHeight();
    const double last = this->currentHeight();

    setStylePrivate(fast, __style, style);

    if(fast){
        setcolor_struct(&style.colore, TEMP_COLOR);
        style.thickness =  widthToPressure(TEMP_TICK);
    }

    memcpy(&tmp_point.m_color, &style.colore, sizeof(style.colore));

    style.nx = (style.nx <= 0) ? 1 : style.nx;
    style.ny = (style.ny <= 0) ? 1 : style.ny;

    deltax = height_p / (double)style.nx;
    deltay = width_p / (double)style.ny;
    ct_del = deltax;

    tmp_point.m_pressure = widthToPressure(style.thickness);

    /* draw the orizzontal line */
    drawLineOrizzontal(this->m_point, tmp_point, style, last, deltax, width_p, ct_del);
    /* draw vertical line */
    drawLineVertical(this->m_point, tmp_point, style, last, deltay, height_p);
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

        style.nx = 1;
        style.ny = 1;
    }else{
        fast = true;

        style.nx = TEMP_SQUARE;
        style.ny = TEMP_SQUARE;
    }
}

static inline double widthToPressure(double v){
    return v/10.0;
}

static void drawLineOrizzontal(QList<point_s> &list,
                            point_s &point,
                            style_struct_S &style,
                            const double &last,
                            double &deltax,
                            const double &width_p,
                            const double &ct_del){
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

static void drawLineVertical(QList<point_s> &list,
                            point_s &point,
                            style_struct_S &style,
                            const double &last,
                            double &deltay,
                            const double &height_p){
    double ct_del;
    uint i;

    point.idtratto = IDVERTICALE;
    ct_del = deltay;

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
