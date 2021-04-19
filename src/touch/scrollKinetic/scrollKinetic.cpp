#include "../tabletcanvas.h"
#include "time.h"

#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <QDebug>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;


static long last_time;

void TabletCanvas::scrollKinetic(){
    last_time = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    qDebug() << "milliseconds since epoch: " << last_time;

}
