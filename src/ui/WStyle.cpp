#include "WStyle.h"
#include "utils/WCommonScript.h"
#include <QString>
#include <QStyle>

void WStyle::setStyle(QWidget *widget)
{
    W_ASSERT(widget);
    const auto currentStyle = WStyle::get_style();
    widget->setStyleSheet(currentStyle);
}


QString WStyle::get_style()
{

    return qstr(

                );
}

QPalette WStyle::get_palette()
{
    return QPalette(QColor(43, 42, 51));
}
