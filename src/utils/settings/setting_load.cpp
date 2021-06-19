#include "setting_load.h"

#include <QSettings>
#include "../setting_define.h"
#include "../../mainwindow.h"
#include "ui_mainwindow.h"

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

static void geometry(MainWindow *parent){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_GEOMETRY);

    QRect value_rect = setting.value(KEY_GEOMETRY, parent->geometry()).toRect();
    parent->setGeometry(value_rect);

    setting.endGroup();
}

void MainWindow::loadPenOrMouse(){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_INSERT_METHOD_PEN_MOUSE);
    touch_or_pen = setting.value(KEY_INSERT_METHOD_PEN_MOUSE, false).toBool();
    setting.endGroup();
    update_touch_or_pen();
}

void setting_load(MainWindow *parent)
{
    geometry(parent);
    setting_load_redoundo(parent);
    setting_load_list_hidden(parent);
    parent->loadPenOrMouse();
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
