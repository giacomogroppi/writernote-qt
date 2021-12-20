#include "mainwindow.h"
#include <QTranslator>
#include <QStringList>
#include <QInputDialog>

#include "utils/dialog_critic/dialog_critic.h"
#include "touch/tabletapplication.h"

#include "changeLanguage.h"
#include "setting_define.h"

#include <QSettings>

#define IT "Italiano"
#define EN "English"

QString language_manager::loadLastLanguage(){
    QString __res;

    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_LANGUAGE);
    __res = setting.value(KEY_LAST_LANGUAGE, EN).toString();

    setting.endGroup();

    return __res;
}

void language_manager::saveCurrentLanguage(QString &language){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LAST_LANGUAGE);

    setting.setValue(KEY_LAST_LANGUAGE, language);

    setting.endGroup();
}

void language_manager::setLanguage(QCoreApplication *app)
{
    QString __temp = language_manager::loadLastLanguage();
    QTranslator __t;

    if(__temp != IT)
        return;

    if(!__t.load(":/language/language/it.qm"))
        return dialog_critic("We had a problem loading " + __temp );

    app->installTranslator(&__t);

}

/*
 * now writernote support only
 * it and en
*/

void MainWindow::on_actionChange_language_triggered()
{


    QStringList __l_list;
    QString lan_dect, __last;

    __last = language_manager::loadLastLanguage();

    __l_list << IT << EN;

    lan_dect = QInputDialog::getItem(NULL, "Select your language", "Language", __l_list);

    if(lan_dect == __last)
        return;

    language_manager::saveCurrentLanguage(lan_dect);

    user_message("Ok you need to restart writernote to change your language");


}

