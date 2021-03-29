#include "../tabletcanvas.h"

#include "../../sheet/load_last_style.h"
#include "../../utils/color/setcolor.h"

#define DISTANCEFROMLEFTANDRIGHT 0
#define DEFAULTROTATION 0
#define DEFAULTPOSIZIONEAUDIO 0

#include "../../utils/color/setcolor.h"
#include "../../utils/dialog_critic/dialog_critic.h"

static double widthToPressure(double);

static double width_(datastruct *);

static void addPointZero(currenttitle_class *);

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
    if(!disegnofoglio_bool) return;

    bool fast = false;

    struct point_s temp_point;
    QPointF p_traslation = data->datatouch->scala_all();

    fast_sheet_ui::n_style res;

    struct style_struct_S *style = nullptr;

    double deltax, deltay, temp;

    double last, height_p, width_p;
    int i;

    width_p = width_(data->datatouch);

    height_p = int((double)width_p * double(4.0/3.0));

    if(width_p == INT32_MIN){
        goto error_;
    }

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

    memcpy(&temp_point.m_color, &style->colore, sizeof(colore_s));

    deltax = (double)height_p / (double)style->nx;
    deltay = (double)width_p / (double)style->ny;

    temp = deltax;

    temp_point.m_pressure = widthToPressure(style->thickness);


    temp_point.idtratto = IDORIZZONALE;
    for(i=0; i<style->nx; i++){
        temp_point.m_x = 0;
        temp_point.m_y = last + deltax;

        data->datatouch->m_point.append(temp_point);

        temp_point.m_x = width_p;
        data->datatouch->m_point.append(temp_point);

        deltax += temp;
    }


    temp_point.idtratto = IDVERTICALE;
    temp = deltay;
    for(i=0; i<style->ny; i++){
        temp_point.m_x = deltay;
        temp_point.m_y = last; /* corrisponde to 0 */

        data->datatouch->m_point.append(temp_point);

        temp_point.m_y = height_p + last;
        data->datatouch->m_point.append(temp_point);

        deltay += temp;

    }

    this->disegnofoglio_bool = false;
    this->isloading = true;

    free(style);

    datastruct::inverso(p_traslation);
    data->datatouch->scala_all(p_traslation);

    if(data->datatouch->posizionefoglio.length() == 1)
        this->resizeEvent(nullptr);

    return;

    error_:
    dialog_critic("We had an internal problem, restarting writernote might fix the problem");
    disegnofoglio_bool = false;

}

static style_struct_S * setStylePrivate(bool *fast, fast_sheet_ui::n_style res){

    style_struct_S * mall_style = (style_struct_S *)malloc(sizeof(style_struct_S)), * temporary;

    if(!mall_style)
        return NULL;

    if(res == fast_sheet_ui::empty){
        temporary = load_default_drawing();
        if(temporary == NULL){
            load_default_onlystyle(mall_style);
        }else{
            memcpy(mall_style, temporary, sizeof(style_struct_S));
        }
    }else if(res == fast_sheet_ui::line){
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

    int i, len;

    for(i=0, len = data->m_point.length(); i<len-1; i++){
        if(data->m_point.at(i).idtratto == IDORIZZONALE){
            return (data->m_point.at(i).m_x - data->m_point.at(i+1).m_x);
        }
    }

    double temp_ = (double)INT32_MIN;

    for(i=0; i<len; i++){
        if(data->m_point.at(i).m_x > temp_){
            temp_ = data->m_point.at(i).m_x;
        }
    }


    return temp_;
}



static void addPointZero(currenttitle_class *data){
    if(data->datatouch->isempty()){
        struct point_s temp;
        temp.idtratto = IDTRATTOZERO;
        temp.m_x = 0;
        temp.m_y = 0;

        data->datatouch->m_point.append(temp);
    }
}

