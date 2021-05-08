#include "../mainwindow.h"
#include "restore_file_critic.h"
#include "ui/setting_restore_ui.h"

static bool change;
static bool alreadyconnect = false;

void MainWindow::on_actionRestore_file_triggered()
{

}

void MainWindow::on_actionOption_triggered()
{
    change = false;
    struct __data data;

    data = m_setting->getData();

    /* we don't want to connect the function everytime */
    if(!alreadyconnect){
        QObject::connect(m_setting, &setting_restore_ui::dataChange, [=](){
            change = true;
        });
    }

    m_setting->exec();

    if(change){
        m_setting->setData(&data);
    }

}
