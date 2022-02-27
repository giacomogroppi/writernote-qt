#include "mainwindow.h"
#include "ui_scroll/scroll.h"


static scroll * __scroll = nullptr;

void MainWindow::on_actionScroll_triggered()
{
    if(!__scroll){
        __scroll = new class scroll(this);

        QObject::connect(__scroll, &scroll::updateData, [=](uint enable, int speed){
            _canvas->m_scrolling_speed_enable = enable;
            _canvas->m_scrolling_speed = speed;
        });

    }

    __scroll->reset(_canvas->m_scrolling_speed_enable,
                    _canvas->m_scrolling_speed);

    __scroll->exec();

}
