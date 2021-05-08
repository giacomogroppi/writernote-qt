#include "setting_load.h"

#include <QSettings>
#include "../setting_define.h"

static void setting_load_list_hidden(MainWindow *);

static void setting_load_redoundo(MainWindow *parent){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_REDOUNDO);

    bool data = setting.value(KEY_REDOUNDO, false).toBool();

    if(data)
        parent->ui->actionEnable_redo_undo->setText(REDO_UNDO_DISABLE);
    else
        parent->ui->actionEnable_redo_undo->setText(REDO_UNDO_ENABLE);

    parent->enableredoundo = data;

    setting.endGroup();
}

void setting_load(MainWindow *parent)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY);

    QRect value_rect = setting.value(KEY_GEOMETRY, parent->geometry()).toRect();
    parent->setGeometry(value_rect);

    setting.endGroup();

    setting_load_redoundo(parent);
    setting_load_list_hidden(parent);
}

static void hideinstart(MainWindow *parent, bool check){
    parent->ui->button_left_hide->setHidden(check);
    parent->ui->listWidgetSX->setHidden(check);

    parent->ui->button_right_hide->setHidden(!check);
}

static void setting_load_list_hidden(MainWindow *parent){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_LIST_HIDDEN);

    bool temp = setting.value(KEY_LIST_HIDDEN, false).toBool();

    hideinstart(parent, temp);

    setting.endGroup();
}
