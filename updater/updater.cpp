#include "updater.h"
#include "ui_updater.h"
#include "../src/utils/dialog_critic/dialog_critic.h"

updater::updater(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::updater)
{
    ui->setupUi(this);
}

updater::~updater()
{
    delete ui;
}

bool updater::downloadFile(QString &l)
{
    int res;
    res = system("powershell");

    if(!res){
        dialog_critic("It seams that you don't have installed powershell");
        return false;
    }

}



