#include "../tabletcanvas.h"

#include "../../sheet/load_last_style.h"
#include "../../sheet/setting_color.h"

#define DISTANCEFROMLEFTANDRIGHT 0
#define DEFAULTROTATION 0
#define DEFAULTPOSIZIONEAUDIO 0

#include "../../utils/color/setcolor.h"
#include "../../utils/dialog_critic/dialog_critic.h"

static qreal widthToPressure(int);

static int height_(datastruct *);
static int width_(datastruct *);

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;

    int i, last = 0;
    int lunghezza;

    int height, width;

    width = width_(data->datatouch);
    height = height_(data->datatouch);

    if(width == -1 || height == -1)
        return dialog_critic("We had an internal problem, restarting writer note might fix the problem");

    lunghezza = (long double)NUMEROPIXELPAGINA * data->datatouch->zoom;

    if(data->datatouch->posizionefoglio.length())
       last = data->datatouch->posizionefoglio.last() + (long double)NUMEROPIXELPAGINA * data->datatouch->zoom;

    data->datatouch->posizionefoglio.append(last);

    struct style_struct_S *style = load_default_drawing();
    if(style == NULL){
        style = new style_struct_S;
        load_default_onlystyle(style);
    }

    QColor coloretemp(settaggiocolore(style->colore));

    struct colore_s coloretemp_struttura;

    /* insert a point (0, 0) */
    if(this->data->datatouch->m_point.length() == 0){
        struct point_s temp;
        temp.idtratto = IDTRATTOZERO;
        temp.m_x = 0;
        temp.m_y = 0;

        data->datatouch->m_point.append(temp);

    }

    coloretemp = setcolor(&coloretemp_struttura);
    struct point_s temp_point;

    float pressure_default = widthToPressure(style->thickness);



    int deltax = height / style->nx;
    int deltay = width / style->ny;

    /*
     * orizzonal line
    */
    for(i=0; i<deltax; i++){

    }

    /*for(i = 0, last += 10; i < 61; i++, last += 40){
        temp_point.m_x = DISTANCEFROMLEFTANDRIGHT;
        temp_point.m_y = (double)last;

        memcpy(&temp_point.m_color, &coloretemp_struttura, sizeof(struct colore_s));
        temp_point.m_posizioneaudio = DEFAULTPOSIZIONEAUDIO;
        temp_point.m_pressure = pressure_default;
        temp_point.rotation = DEFAULTROTATION;
        temp_point.idtratto = IDORIZZONALE;

        data->datatouch->m_point.append(temp_point);


        temp_point.m_x = (double)(lunghezza - DISTANCEFROMLEFTANDRIGHT);
        temp_point.m_y = (double)last;

        data->datatouch->m_point.append(temp_point);

    }*/


    /*
     * TODO -> add x drawing
    */
    this->disegnofoglio_bool = false;
    this->isloading = true;
}

static inline qreal widthToPressure(int v){
    return (v-1)*10;
}

static int width_(datastruct *data){
    if(data->posizionefoglio.isEmpty())
        return NUMEROPIXELPAGINA;

    int i, len;

    for(i=0, len = data->m_point.length(); i<len-1; i++){
        if(data->m_point.at(i).idtratto == IDORIZZONALE){
            return data->m_point.at(i+1).m_x;
        }
    }

    return -1;
}
static int height_(datastruct *data){
    if(data->posizionefoglio.isEmpty())
        return NUMEROPIXELORIZZONALI;

    int i, len;
    for(i=0, len = data->m_point.length(); i<len-1;i++){
        if(data->m_point.at(i).idtratto == IDVERTICALE){
            return data->m_point.at(i+1).m_y;
        }

    }

    int temp_ = INT32_MIN;

    /*
     * there are points with horizons
    */
    for(i=0; i<len; i++){
        if(data->m_point.at(i).m_y > temp_
                && data->m_point.at(i).idtratto != IDORIZZONALE){
            temp_ = data->m_point.at(i).m_y;
        }
    }

    /*
     * temp_ now contains the largest value with the IDORIZZONALE id written
    */

    return temp_;
}
