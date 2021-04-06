#include "../mainwindow.h"
#include <QTranslator>
#include <QStringList>
#include <QInputDialog>

/*
 * now writernote support only
 * it and en
*/

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
        __t.load(":/language/language/it.qm");
    }




}
