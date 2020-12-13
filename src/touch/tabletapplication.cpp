#include "tabletapplication.h"

//! [0]
bool TabletApplication::event(QEvent *event)
{
    if (event->type() == QEvent::TabletEnterProximity ||
        event->type() == QEvent::TabletLeaveProximity) {
        return true;
    }
    return QApplication::event(event);
}
//! [0]
