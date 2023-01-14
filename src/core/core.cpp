#include "core.h"
#include "utils/WCommonScript.h"
#include "utils/platform.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include <QPalette>
#include <QCursor>

bool __is_mobile_view = false;
class MainWindow *__private_mainwindow = nullptr;
class TabletCanvas *__private_canvas = nullptr;

bool core::is_dark_mode()
{
    const auto not_used name = "core::is_dark_mode()";
    constexpr int min = 100;
    int r, g, b;
    const auto main = core::get_main_window();
    const QColor color = main->palette().color(main->backgroundRole());
    color.getRgb(&r, &g, &b);

    return r < min and g < min and b < min;
}


QPoint core::get_pos_start_mouse()
{
    return QCursor::pos();
}
