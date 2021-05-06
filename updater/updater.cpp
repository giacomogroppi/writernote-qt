#include "updater.h"
#include "ui_updater.h"
#include "../src/utils/dialog_critic/dialog_critic.h"

#define OK_POWER_EXT 0
#define OK_POWER_DOWN 0

#include <QFile>
#include <QDir>

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

bool updater::downloadFile(QString url, QString dest)
{
    int res;
    res = system("powershell");

    if(!res){
        dialog_critic("It seams that you don't have installed powershell");
        return false;
    }

    url = "Invoke-WebRequest -uri " + url + " -Method \"GET\"  -Outfile " + dest;

    res = system(url.toUtf8().constData());

    if(res != OK_POWER_DOWN){
        return false;
    }
    return true;

}

bool updater::exstractFile(QString l, const QString &dest)
{
    int res;

    l = "Expand-Archive -Path " + l + " -destinationPath " + dest;

    res = system(l.toUtf8().constData());

    if(res != OK_POWER_EXT){
        return true;
    }
    return false;
}

#define POS (QString)"C:\\Program Files (x86)\\writernote\\"

void updater::downloadUpdate()
{
    QDir __dir(POS + "\\writernote.exe");

    if(!QFile::exists(POS)){
        if(__dir.exists()){
            goto go;
        }

        return dialog_critic("I cant find writernote in " + POS);

    }


    go:



    return;
}



