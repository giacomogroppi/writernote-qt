#include "../mainwindow.h"
#include <QTranslator>
#include <QStringList>
#include <QInputDialog>

#include "../utils/dialog_critic/dialog_critic.h"
#include "../touch/tabletapplication.h"

/*
 * now writernote support only
 * it and en
*/

TabletApplication *__app;

#define IT "Italiano"
#define EN "English"

void MainWindow::on_actionChange_language_triggered()
{
    QTranslator __t;

    QStringList __l_list;
    QString lan_dect;

    __l_list << IT << EN;

    lan_dect = QInputDialog::getItem(NULL, "Select your language", "Language", __l_list);


    if(lan_dect == IT){
        if(!__t.load(":/language/language/it.qm"))
            return dialog_critic("We had a problem loading " + lan_dect );
    }

    if(lan_dect != EN)
        __app->installTranslator(&__t);

    this->update();

}


void MainWindow::setApplication(TabletApplication *_app){
    __app = _app;
}
