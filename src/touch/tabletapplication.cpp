#include "tabletapplication.h"

bool TabletApplication::event(QEvent *event)
{
    if (event->type() == QEvent::TabletEnterProximity ||
        event->type() == QEvent::TabletLeaveProximity) {
        m_canvas->setTabletDevice(static_cast<QTabletEvent *>(event));
        return true;
    }
    return QApplication::event(event);
}

