#include "setting_load.h"

#include <QSettings>
#include "../setting_define.h"

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

static void setting_load_auto_save(MainWindow *parent){
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_AUTOSAVE);

    parent->autosave = setting.value(KEY_AUTOSAVE, false).toBool();

    setting.endGroup();
}

void setting_load(MainWindow *parent)
{
    setting_load_redoundo(parent);
    setting_load_auto_save(parent);
}
