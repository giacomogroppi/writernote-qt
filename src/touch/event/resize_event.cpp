#include "../tabletcanvas.h"

static void checkPosition(datastruct *);

void TabletCanvas::resizeEvent(QResizeEvent *)
{
    if(data == NULL)
        return;

    checkPosition(data->datatouch);

    if(!data->datatouch->isempty()){
        unsigned int i = data->datatouch->posIdOrizzonal();

        if(!i)
            goto init_;

        double res;
        res = data->datatouch->m_point.at(i).m_x;

        if(res < 0)
            data->datatouch->scala_x(-res);

        data->datatouch->adjastHeight(height());

    }

    init_:

    initPixmap(true);
    this->isloading = true;
}


static void checkPosition(datastruct *data){
    if(data->isempty())
        return;

    point_s & __point = data->m_point.first();
    double res;

    if(__point.m_x <= 0)
    return;
    res = __point.m_x;

    data->scala_x(-res);

}

