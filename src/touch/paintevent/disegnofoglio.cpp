#include "../tabletcanvas.h"

#include "../../sheet/load_last_style.h"
#include "../../utils/color/setcolor.h"

#define DISTANCEFROMLEFTANDRIGHT 0
#define DEFAULTROTATION 0
#define DEFAULTPOSIZIONEAUDIO 0

#include "../../utils/color/setcolor.h"
#include "../../utils/dialog_critic/dialog_critic.h"

static qreal widthToPressure(int);

static double height_(datastruct *);
static double width_(datastruct *);

static void addPointZero(currenttitle_class *);
static double getLastPoint(currenttitle_class *);


/*
 * remove this define and add option to change
 * color, size line
*/
#define TEMP_COLOR Qt::black
#define TEMP_TICK 1
#define TEMP_N_X 40
#define TEMP_SQUARE 40

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;

    double last, height_p, width_p;
    int i;

    width_p = width_(data->datatouch);
    height_p = height_(data->datatouch);

    if(width_p == INT32_MIN || height_p == INT32_MIN){
        dialog_critic("We had an internal problem, restarting writernote might fix the problem");
        disegnofoglio_bool = false;
    }

    /* he get the last point draw */
    last = getLastPoint(data);

    data->datatouch->posizionefoglio.append(last);

    struct style_struct_S *style = nullptr;

    auto res = this->m_sheet->WhatIsSelected();

    bool fast = false;

    if(res != fast_sheet_ui::empty)
        style = (style_struct_S *)malloc(sizeof(style_struct_S));


    if(res == fast_sheet_ui::empty){
        style = load_default_drawing();
        if(style == NULL){
            style = new style_struct_S;
            load_default_onlystyle(style);
        }
    }else if(res == fast_sheet_ui::line){
        fast = true;

        style->nx = TEMP_N_X;
        style->ny = 0;
    }
    else if(res == fast_sheet_ui::square){
        fast = true;

        style->nx = TEMP_SQUARE;
        style->ny = TEMP_SQUARE;
    }
    else if(res == fast_sheet_ui::white){
        /* we set the color manually */
        setcolor_struct(&style->colore, Qt::black);

        style->nx = 1;
        style->ny = 1;
    }else{
        free(style);
        this->disegnofoglio_bool = false;
        return dialog_critic("You have to change style. This is\nnot already supported in writernote");
    }

    if(fast){
        setcolor_struct(&style->colore, TEMP_COLOR);
        style->thickness =  widthToPressure(TEMP_TICK);
    }


    /* insert a point (0, 0) */
    addPointZero(data);


    struct point_s temp_point;
    memcpy(&temp_point.m_color, &style->colore, sizeof(colore_s));

    double deltax = height_p / style->nx;
    double deltay = (double)width_p / (double)style->ny;

    double temp = deltax;

    temp_point.m_pressure = widthToPressure(style->thickness);

    for(i=0; i<style->nx; i++){
        temp_point.idtratto = IDORIZZONALE;
        temp_point.m_x = 0;
        temp_point.m_y = last + deltax;

        data->datatouch->m_point.append(temp_point);

        temp_point.m_x = width_p;
        data->datatouch->m_point.append(temp_point);

        deltax += temp;
    }


    temp = deltay;
    for(i=0; i<style->ny; i++){
        temp_point.idtratto = IDVERTICALE;
        temp_point.m_x = deltay;
        temp_point.m_y = last; /* corrisponde to 0 */

        data->datatouch->m_point.append(temp_point);

        temp_point.m_y = height_p + last;
        data->datatouch->m_point.append(temp_point);

        deltay += temp;

    }

    this->disegnofoglio_bool = false;
    this->isloading = true;

    if(res != fast_sheet_ui::empty){
        free(style);
    }

}

static inline qreal widthToPressure(int v){
    return (v-1)*10;
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
            return data->m_point.at(i+1).m_x;
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

static double height_(datastruct *data){
    if(data->posizionefoglio.isEmpty())
        return NUMEROPIXELORIZZONALI;

    int i, len;
    for(i=0, len = data->m_point.length(); i<len-1;i++){
        if(data->m_point.at(i).idtratto == IDVERTICALE){
            return data->m_point.at(i+1).m_y;
        }
    }

    double temp_ = (double)INT32_MIN;

    /*
     * there are points with horizons
    */
    for(i=0; i<len; i++){
        if(data->m_point.at(i).m_y > temp_){
            temp_ = data->m_point.at(i).m_y;
        }
    }

    /*
     * temp_ now contains the largest value in y written
    */

    return temp_;
}

static void addPointZero(currenttitle_class *data){
    if(data->datatouch->m_point.length() == 0){
        struct point_s temp;
        temp.idtratto = IDTRATTOZERO;
        temp.m_x = 0;
        temp.m_y = 0;

        data->datatouch->m_point.append(temp);
    }
}


static double getLastPoint(currenttitle_class *data){
    if(data->datatouch->m_point.isEmpty())
        return (double) 0;

    int i, len;
    double p = data->datatouch->m_point.first().m_y;

    len = data->datatouch->m_point.length();
    for(i=0; i<len; i++){
        if(data->datatouch->m_point.at(i).m_y > p)
            p = data->datatouch->m_point.at(i).m_y;
    }

    return p;
}
