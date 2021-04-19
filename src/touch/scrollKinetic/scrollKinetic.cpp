#include "../tabletcanvas.h"
#include "time.h"

#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <QDebug>
#include <QTimer>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;


static long last_time;
static QTimer * timer = nullptr;

static inline long current_time(){
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

void TabletCanvas::scrollKinetic(QPointF first, QPointF second){
    if(!timer){
        timer = new QTimer(this);
        QObject::connect(timer, &QTimer::timeout, [=]{

        });
    }
    long delta;
    int speed;
    delta = last_time - current_time();



}

void TabletCanvas::updateTimeScroll(){
    last_time = current_time();
}
