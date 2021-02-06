#ifndef ZOOM_CONTROL_H
#define ZOOM_CONTROL_H

#include "../datastruct/datastruct.h"
#include <QPointF>

class zoom_control
{
private:
    datastruct *data;

    /*
     *  la funzione prense il punto passatogli
     *  e sposta il punto in (1,1) per facilitare i conti
     *  -> dopo risposta il tutto
     *
     *  true -> inverse operation
    */
    void trasla(QPointF &, long double delta = 0);

public:
    zoom_control();

    void settingData(datastruct *data_struct){data = data_struct;}

    void zoom(QPointF &, long double);

};

#endif // ZOOM_CONTROL_H
