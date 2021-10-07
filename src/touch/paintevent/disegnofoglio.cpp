#include "../tabletcanvas.h"
#include "../../utils/color/setcolor.h"

#include <QDebug>

#define DISTANCEFROMLEFTANDRIGHT 0
#define DEFAULTROTATION 0
#define DEFAULTPOSIZIONEAUDIO 0

#include "../../utils/color/setcolor.h"
#include "../../utils/dialog_critic/dialog_critic.h"

static double widthToPressure(double);

static void addPointZero(Document *);
static void drawLineOrizzontal(QList<point_s> &list,
                               point_s &point,
                               style_struct_S &style,
                               const double &last,
                               double &deltax,
                               const double &width_p,
                               const double &ct_del);

static void drawLineVertical(QList<point_s> &data,
                         point_s &point,
                         style_struct_S &style,
                         const double &last,
                         double &deltay,
                         const double &height_p);

/*
 * remove this define and add option to change
 * color, size line
*/
#define TEMP_COLOR Qt::black
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

static void setStylePrivate(bool &, fast_sheet_ui::n_style, style_struct_S &);

void TabletCanvas::disegnafoglio(){
    bool fast = false, need_scale = false;
    struct point_s temp_point;
    struct style_struct_S style;
    static fast_sheet_ui::n_style res;
    static QList<point_s> point;
    static double deltax, deltay, ct_del, last, height_p, width_p;

    point.clear();

    if(!disegnofoglio_bool)
        return;

    if(!data->datatouch->isempty()){
        need_scale = true;
        data->datatouch->scala_all();
    }
    width_p = data->datatouch->currentWidth();

    height_p = int((double)width_p * double(4.0/3.0));

    /* he get the last point draw */
    last = data->datatouch->biggery();

    data->datatouch->posizionefoglio.append(last);

    res = m_sheet->WhatIsSelected();
    /* set style value */
    setStylePrivate(fast, res, style);

    if(fast){
        setcolor_struct(&style.colore, TEMP_COLOR);
        style.thickness =  widthToPressure(TEMP_TICK);
    }

    /* insert a point (0, 0) */
    addPointZero(data);
    memcpy(&temp_point.m_color, &style.colore, sizeof(style.colore));

    style.nx = (style.nx <= 0) ? 1 : style.nx;
    style.ny = (style.ny <= 0) ? 1 : style.ny;

    deltax = height_p / (double)style.nx;
    deltay = width_p / (double)style.ny;
    ct_del = deltax;

    temp_point.m_pressure = widthToPressure(style.thickness);

    /* draw the orizzontal line */
    drawLineOrizzontal(point, temp_point, style, last, deltax, width_p, ct_del);
    /* draw vertical line */
    drawLineVertical(point, temp_point, style, last, deltay, height_p);

    this->data->datatouch->appendToTop(point);

    this->disegnofoglio_bool = false;

    if(need_scale)
        data->datatouch->restoreLastTranslation();

    if(data->datatouch->posizionefoglio.length() == 1)
        this->resizeEvent(nullptr);
}

static void setStylePrivate(bool &fast, fast_sheet_ui::n_style res, style_struct_S &style){
    if(res == fast_sheet_ui::empty){
        res = fast_sheet_ui::square;
    }

    if(res == fast_sheet_ui::line){
        fast = true;

        style.nx = TEMP_N_X;
        style.ny = 0;
    }
    else if(res == fast_sheet_ui::white){
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

static void addPointZero(Document *data){
    if(data->datatouch->isempty()){
        struct point_s temp;
        temp.idtratto = IDTRATTOZERO;
        temp.m_x = 0;
        temp.m_y = 0;

        data->datatouch->append(temp);
    }
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
