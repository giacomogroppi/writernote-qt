#include "../../mainwindow.h"
#include "ui_scroll/ui_scroll.h"


static ui_scroll * __scroll = nullptr;

void MainWindow::on_actionScroll_triggered()
{
    if(!__scroll){
        __scroll = new ui_scroll(this);

        QObject::connect(__scroll, &ui_scroll::updateData, [=](uint enable, int speed){
            m_canvas->m_scrolling_speed_enable = enable;
            m_canvas->m_scrolling_speed = speed;
        });

    }

    __scroll->reset(m_canvas->m_scrolling_speed_enable,
                    m_canvas->m_scrolling_speed);

    __scroll->exec();

}
