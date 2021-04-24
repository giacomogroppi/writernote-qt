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
static double speed_x, speed_y;
static long delta_time;

static int how_time = 0;

static inline long current_time(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

#define controll_speed(speed, x, comparison) \
    speed = (speed comparison x) ? x : speed;

#define def_comp 1000.0

#define cont_scoll(speed, valore) \
    (speed >= valore)

#define def_val 1.0

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

                speed_x /= 1.05;
                speed_y /= 1.05;

                /*
                speed_x /= double((m_scrolling_speed)/2);
                speed_y /= double((m_scrolling_speed)/2);*/

                qDebug() << "delta x " << speed_x << "delta y " << speed_y;

                __delta = (double) speed_x;
                if(cont_scoll(__delta, def_val)
                        && scroll::itspossibletoscrollx(data->datatouch, m_pixmap.width(), & __delta)){
                    ismoving.set = 1;
                }

                ismoving.point.setX(__delta);

                __delta = (double) speed_y;
                if(cont_scoll(__delta, def_val)
                        && scroll::itspossibletoscrolly(data->datatouch, m_pixmap.height(), &__delta)){
                    ismoving.set = 1;
                    qDebug() << "can scroll y";
                }else{
                    qDebug() << "can't scroll y";
                }

                ismoving.point.setY(__delta);

                if(!ismoving.set){
                    qDebug() << "Stoppato il timer";
                    timer->stop();
                    return;
                }


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

    speed_x *= -100.0;
    speed_y *= -100.0;


    /*controll_speed(speed_x, def_comp, >);
    controll_speed(speed_y, def_comp, >);
    controll_speed(speed_x, def_comp, <);
    controll_speed(speed_y, def_comp, <);*/

    how_time = m_scrolling_speed * 100.0;

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
