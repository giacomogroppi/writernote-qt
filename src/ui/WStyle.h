#pragma once

#include <QWidget>
#include <QPalette>

namespace WStyle{
    void setStyle(QWidget *widget);
    QString get_style();
    QPalette get_palette();
};
