#include "uicore.h"

void UiCore::makePop(QWidget *widget)
{
    widget->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
}
