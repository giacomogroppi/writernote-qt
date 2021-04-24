#include "../tabletcanvas.h"
#include "time.h"

#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <QDebug>
#include <QTimer>

#include "../event/itspossibletoscroll.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;


static long last_time, __last_time;
static QTimer * timer = nullptr;
static int speed_x, speed_y;
static long delta_time;

static int how_time = 0;

static inline long current_time(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void TabletCanvas::scrollKinetic(QPointF first, QPointF second){
    int delta_x, delta_y;
    if(!timer){
        timer = new QTimer(this);
        QObject::connect(timer, &QTimer::timeout, [=]{

            //qDebug() << "Inside: " << how_time << delta_time << speed_x << speed_y;

            double __delta;
            ismoving.set = 0;
            ismoving.point = QPointF(0, 0);

            if(how_time){
                delta_time /= this->m_scrolling_speed;
                timer->start(1);

                speed_x /= ((m_scrolling_speed)/2);
                speed_y /= ((m_scrolling_speed)/2);

                __delta = (double) speed_x;
                if(!scroll::itspossibletoscrollx(data->datatouch, m_pixmap.width(), & __delta)){
                    ismoving.set = 1;
                    qDebug() << "Non posso scorrere first";
                }

                ismoving.point.setX(__delta);

                __delta = (double) speed_y;
                if(!scroll::itspossibletoscrolly(data->datatouch, m_pixmap.height(), &__delta)){
                    ismoving.set = 1;
                    qDebug() << "Non posso scorrere second";
                }

                if(!ismoving.set){
                    timer->stop();
                    return;
                }

                qDebug() << ismoving.point;

                ismoving.point.setY(__delta);

                this->ismoving_f();


            }else{
                timer->stop();
            }

            -- how_time;

        });
    }

    timer->stop();

    delta_time = last_time - __last_time;

    ++ delta_time;

    delta_x = first.x() - second.x();
    delta_y = first.y() - second.y();

    speed_x = delta_x / delta_time;
    speed_y = delta_y / delta_time;

    speed_x *= 100;
    speed_y *= 100;

    how_time = m_scrolling_speed * 100;

    qDebug() << "Outside: " << how_time << "delta time " <<delta_time <<
             "speed x " << speed_x <<
             "speed y" << speed_y <<
             "delta x" << delta_x <<
             "delta y" << delta_y;

    timer->start(1);
}

void TabletCanvas::updateTimeScroll(){
    if(timer)
        timer->stop();

    __last_time = last_time;
    last_time = current_time();
}
