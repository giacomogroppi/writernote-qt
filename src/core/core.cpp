#include "core.h"
#include "utils/common_script.h"
#include "utils/platform.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QWidget>
#include <QPalette>


static void adjustButtonDark(QAction *action, const QString &_path)
{
    constexpr auto suff = "_black_theme";
    constexpr auto find = ".png";

    QString path = _path;
    path = path.insert(_path.indexOf(find), suff);
    const auto icon = action->icon();

    qDebug() << icon;
}

bool __is_mobile_view = false;
class MainWindow *__private_mainwindow;

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

void core::adjustDarkMode()
{
    auto *main = core::get_main_window()->ui;
    adjustButtonDark(main->actionpen, "");
}
