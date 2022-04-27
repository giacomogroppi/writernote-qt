#include "uicore.h"

void UiCore::makePop(QWidget *widget)
{
    widget->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
}

void UiCore::makeZeroBorder(QWidget *widget)
{
    widget->setStyleSheet("QWidget { border : 0px solid black;}");
}
