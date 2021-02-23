#include "../tabletcanvas.h"

#include "../../sheet/load_last_style.h"
#include "../../sheet/setting_color.h"

#define DISTANCEFROMLEFTANDRIGHT 0
#define DEFAULTROTATION 0
#define DEFAULTPOSIZIONEAUDIO 0

#include "../../utils/color/setcolor.h"

static qreal widthToPressure(int);

void TabletCanvas::disegnafoglio(){
    if(!disegnofoglio_bool) return;

    int i, last = 0;
    int lunghezza;

    /*lunghezza = this->m_pixmap.width();*/
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

        /*
         * last data struct
        */
        /*this->data->datatouch->x.append(0);
        this->data->datatouch->y.append(0);
        this->data->datatouch->idtratto.append(IDTRATTOZERO);


        this->data->datatouch->color.append(coloretemp_struttura);
        this->data->datatouch->pressure.append(0);
        this->data->datatouch->rotation.append(DEFAULTROTATION);
        this->data->datatouch->posizioneaudio.append(0);*/
    }

    coloretemp = setcolor(&coloretemp_struttura);
    struct point_s temp_point;

    float pressure_default = widthToPressure(style->thickness);

    for(i = 0, last += 10; i < 61; i++, last += 40){
        /* starting point */
        temp_point.m_x = DISTANCEFROMLEFTANDRIGHT;
        temp_point.m_y = (double)last;

        memcpy(&temp_point.m_color, &coloretemp_struttura, sizeof(struct colore_s));
        temp_point.m_posizioneaudio = DEFAULTPOSIZIONEAUDIO;
        temp_point.m_pressure = pressure_default;
        temp_point.rotation = DEFAULTROTATION;
        temp_point.idtratto = IDORIZZONALE;

        data->datatouch->m_point.append(temp_point);

        /*
         * last data struct
        */
        /*
        this->data->datatouch->x.append(DISTANCEFROMLEFTANDRIGHT);
        this->data->datatouch->y.append((double)last);
        this->data->datatouch->color.append(coloretemp_struttura);
        this->data->datatouch->posizioneaudio.append(DEFAULTPOSIZIONEAUDIO);

        this->data->datatouch->pressure.append(widthToPressure(style->thickness));
        this->data->datatouch->rotation.append(DEFAULTROTATION);

        this->data->datatouch->idtratto.append(IDORIZZONALE);*/

        temp_point.m_x = (double)(lunghezza - DISTANCEFROMLEFTANDRIGHT);
        temp_point.m_y = (double)last;

        data->datatouch->m_point.append(temp_point);
        /*
         * last point touch
        */
        /*
        // End of the line
        this->data->datatouch->x.append((double)(lunghezza - DISTANCEFROMLEFTANDRIGHT));
        this->data->datatouch->y.append((double)last);
        this->data->datatouch->color.append(coloretemp_struttura);
        this->data->datatouch->posizioneaudio.append(DEFAULTPOSIZIONEAUDIO);

        this->data->datatouch->pressure.append(widthToPressure(style->thickness));
        this->data->datatouch->rotation.append(DEFAULTROTATION);

        this->data->datatouch->idtratto.append(IDORIZZONALE);*/

    }

    /*
    this->data->datatouch->x.append(0);
    this->data->datatouch->y.append((double)last);
    this->data->datatouch->color.append(coloretemp_struttura);
    this->data->datatouch->posizioneaudio.append(DEFAULTPOSIZIONEAUDIO);

    this->data->datatouch->pressure.append(widthToPressure(style->thickness));
    this->data->datatouch->rotation.append(DEFAULTROTATION);

    this->data->datatouch->idtratto.append(IDVERTICALE);

    this->data->datatouch->x.append((double)lunghezza);
    this->data->datatouch->y.append((double)last);
    this->data->datatouch->color.append(coloretemp_struttura);
    this->data->datatouch->posizioneaudio.append(DEFAULTPOSIZIONEAUDIO);

    this->data->datatouch->pressure.append(widthToPressure(style->thickness));
    this->data->datatouch->rotation.append(DEFAULTROTATION);

    this->data->datatouch->idtratto.append(IDVERTICALE);
*/
    /*
     * TODO -> add x drawing
    */
    this->disegnofoglio_bool = false;
    this->isloading = true;
}

static inline qreal widthToPressure(int v){
    return (v-1)*10;
}
