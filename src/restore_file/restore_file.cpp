#include "mainwindow.h"
#include "qguiapplication.h"
#include "restore_file_critic.h"
#include "ui/setting_restore_ui.h"
#include "ui_restore/restore.h"
#include "core/core.h"

static bool change;
static bool alreadyconnect = false;

/*
 * restore file dialog
*/
void MainWindow::on_actionRestore_file_triggered()
{
    restore __r(nullptr, m_path);
    __r.exec();
}

void MainWindow::on_actionOption_triggered()
{
    change = false;
    const struct __data data = m_setting->getData();

    /* we don't want to connect the function everytime */
    if(!alreadyconnect){
        QObject::connect(m_setting, &setting_restore_ui::dataChange, [=](){
            change = true;
        });

        alreadyconnect = true;
    }

    core::set_max_size_as_screen(m_setting);

    m_setting->exec();

    if(!change){
        m_setting->setData(&data);
    }

}
