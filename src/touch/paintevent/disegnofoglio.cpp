#include "../tabletcanvas.h"

#include "../../sheet/load_last_style.h"
#include "../../utils/color/setcolor.h"

#include <QDebug>

#define DISTANCEFROMLEFTANDRIGHT 0
#define DEFAULTROTATION 0
#define DEFAULTPOSIZIONEAUDIO 0

#include "../../utils/color/setcolor.h"
#include "../../utils/dialog_critic/dialog_critic.h"

static double widthToPressure(double);

static double width_(datastruct *);

static void addPointZero(currenttitle_class *);
static void drawLineOrizzontal(datastruct *data,
                         point_s *point,
                         style_struct_S *style,
                         const double &last,
                         double &deltax,
                         const double &width_p,
                         const double &ct_del);

static void drawLineVertical(datastruct *data,
                         point_s *point,
                         style_struct_S *style,
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

static style_struct_S * setStylePrivate(bool *, fast_sheet_ui::n_style);

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool)
        return;

    bool fast = false, need_scale = false;

    struct point_s temp_point;
    struct style_struct_S *style = nullptr;
    fast_sheet_ui::n_style res;

    double deltax, deltay, ct_del, last, height_p, width_p;

    if(!data->datatouch->isempty()){
        need_scale = true;
        data->datatouch->scala_all(m_from_pdf);
    }

    width_p = width_(data->datatouch);

    height_p = int((double)width_p * double(4.0/3.0));

    /* he get the last point draw */
    last = data->datatouch->biggery();

    data->datatouch->posizionefoglio.append(last);

    res = this->m_sheet->WhatIsSelected();
    /* set style value */
    style = setStylePrivate(&fast, res);

    if(!style){
        goto error_;
    }
    if(fast){
        setcolor_struct(&style->colore, TEMP_COLOR);
        style->thickness =  widthToPressure(TEMP_TICK);
    }

    /* insert a point (0, 0) */
    addPointZero(data);
    memcpy(&temp_point.m_color, &style->colore, sizeof(style->colore));


    if(style->nx <= 0)
        style->nx = 1;

    if(style->ny <= 0)
        style->ny = 1;
    deltax = height_p / (double)style->nx;
    deltay = width_p / (double)style->ny;
    ct_del = deltax;

    temp_point.m_pressure = widthToPressure(style->thickness);

    /* draw the orizzontal line */
    drawLineOrizzontal(data->datatouch, &temp_point, style, last, deltax, width_p, ct_del);
    /* draw vertical line */
    drawLineVertical(data->datatouch, &temp_point, style, last, deltay, height_p);

    this->disegnofoglio_bool = false;
    this->isloading = true;
    free(style);

    if(need_scale)
        data->datatouch->restoreLastTranslation(m_from_pdf);

    if(data->datatouch->posizionefoglio.length() == 1)
        this->resizeEvent(nullptr);

    return;

    error_:
    dialog_critic("We had an internal problem, restarting writernote might fix the problem");
    disegnofoglio_bool = false;

}

static style_struct_S * setStylePrivate(bool *fast, fast_sheet_ui::n_style res){

    style_struct_S * mall_style;

    mall_style = (style_struct_S *)malloc(sizeof(style_struct_S));

    if(!mall_style)
        return NULL;

    if(res == fast_sheet_ui::empty){
        res = fast_sheet_ui::square;
    }

    if(res == fast_sheet_ui::line){
        *fast = true;

        mall_style->nx = TEMP_N_X;
        mall_style->ny = 0;
    }
    else if(res == fast_sheet_ui::square){
        *fast = true;

        mall_style->nx = TEMP_SQUARE;
        mall_style->ny = TEMP_SQUARE;
    }
    else if(res == fast_sheet_ui::white){
        /* we set the color manually */
        setcolor_struct(&mall_style->colore, Qt::black);

        mall_style->nx = 1;
        mall_style->ny = 1;
    }else{
        free(mall_style);
        dialog_critic("You have to change style. This is\nnot already supported in writernote");

        return NULL;
    }

    return mall_style;
}


static inline double widthToPressure(double v){
    return v/10.0;
}

/*
 * to maintain compatibility with all devices it is not so important to draw the new sheet in a fixed size.
 * however, it is important to draw the sheet with the same shape as the one drawn previously.
 * not to break the smoothness and all the features
*/

static double width_(datastruct *data){
    if(data->posizionefoglio.isEmpty())
        return NUMEROPIXELPAGINA;

    uint i, len;

    len = data->length();

    for(i=0; i<len-1; i++){
        if(data->at(i)->idtratto == IDORIZZONALE){
            return (data->at(i+1)->m_x - data->at(i)->m_x);
        }
    }

    return data->biggerx();
}



static void addPointZero(currenttitle_class *data){    
    if(data->datatouch->isempty()){
        struct point_s temp;
        temp.idtratto = IDTRATTOZERO;
        temp.m_x = 0;
        temp.m_y = 0;

        data->datatouch->append(temp);
    }
}

static void drawLineOrizzontal(datastruct *data,
                            point_s *point,
                            style_struct_S *style,
                            const double &last,
                            double &deltax,
                            const double &width_p,
                            const double &ct_del){
    //qDebug() << deltax;

    uint i;
    point->idtratto = IDORIZZONALE;

    for(i=0; i< (uint)style->nx; ++i){
        point->m_x = 0;
        point->m_y = last + deltax;

        data->append(point);

        point->m_x = width_p;
        data->append(point);

        deltax += ct_del;
    }
}

static void drawLineVertical(datastruct *data,
                            point_s *point,
                            style_struct_S *style,
                            const double &last,
                            double &deltay,
                            const double &height_p){
    double ct_del;
    uint i;

    point->idtratto = IDVERTICALE;
    ct_del = deltay;

    //qDebug() << "drawLineVerical" << deltay <<  ct_del;

    for(i=0; i< (uint)style->ny; i++){
        point->m_x = deltay;
        point->m_y = last; /* corrisponde to 0 */

        data->append(point);

        point->m_y = height_p + last;
        data->append(point);

        deltay += ct_del;

    }
}
