#include "core/core.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "button/button_size.h"
#include <QSettings>
#include "utils/setting_define.h"

static void update_size(int size, MainWindow *parent);
static void set_size(int &size);
static void save_setting(const int size);
static int size_res;

void MainWindow::setSizeButton(int size)
{
    if(size == -1){
        set_size(size_res);
    }
    if(size == -2){
        button_size dialog(nullptr, size_res);
        set_size(size_res);

        QObject::connect(&dialog, &button_size::update_size, [=](int val){
            update_size(val, this);
            size_res = val;
        });

        core::set_max_size_as_screen(&dialog);
        dialog.exec();
    }

    update_size(size_res, this);
    save_setting(size_res);
}

static void update_size(int size, MainWindow *parent)
{
    const QSize Size = QSize(size, size);
    parent->ui->mainbar->setIconSize(Size);
    parent->ui->simpleactionbar->setIconSize(Size);
    parent->ui->audiobar->setIconSize(Size);
    parent->ui->toolbarmatita->setIconSize(Size);
    parent->ui->toolBarcolore->setIconSize(Size);
    parent->ui->toolSheet->setIconSize(Size);
}

static void set_size(int &size)
{
    bool ok;
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_SIZE_BUTTON);
    size = setting.value(KEY_SIZE_BUTTON, 20).toInt(&ok);

    if(!ok){
        size = 20;
    }
    setting.endGroup();
}

void MainWindow::on_actionSize_button_triggered()
{
    this->setSizeButton(-2);
}

static void save_setting(const int size)
{
    QSettings setting(ORGANIZATIONAME, APPLICATION_NAME);
    setting.beginGroup(GROUPNAME_SIZE_BUTTON);
    setting.setValue(KEY_SIZE_BUTTON, size);
    setting.endGroup();
}
