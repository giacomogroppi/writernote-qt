#include "updater.h"
#include "ui_updater.h"
#include "../src/utils/dialog_critic/dialog_critic.h"
#include "../src/utils/path/get_path.h"
#include "../src/utils/areyousure/areyousure.h"
#include "../src/utils/get_file_dir/get_file_dir.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QProcess>
#include <QDebug>
#define POS (QString)"C:\\Program Files (x86)\\writernote\\"

updater::updater(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::updater)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager();
    request.setUrl(QUrl("https://api.github.com/repos/giacomogroppi/writernote-qt/tags"));
    reply = manager->get(request);

    QObject::connect(reply, &QNetworkReply::finished, this, &updater::downloadUpdate);

}

updater::~updater()
{
    delete ui;
}

bool updater::downloadFile(const QString &url, const QString &dest)
{
    QList<QString> argv;

    /* 4 min */
    const size_t time = 4*60*1000;

    if(QFile::exists(dest)){
        if(!QFile::remove(dest)){
            user_message("Before update writernote remove this file " + dest);
            return false;
        }
    }

    argv << QString("Invoke-WebRequest -uri %1 -Method \"GET\" -Outfile %2").arg(url).arg(dest);

    return this->exe(argv, time);
}

bool updater::extractFile(const QString &path, const QString &dest)
{
    if(!updater::createDirectory(dest)) return false;
    QList<QString> list = QList<QString>(QString("cd %1; tar -xf %2").arg(dest).arg(path));
    const size_t timeout = /* second */ 10 * 1000;

    if(!updater::cleanDirectory(path)){
        user_message("I can't update writernote becouse I can't remove this folder: " + dest);
        return false;
    }

    int res = this->exe(list, timeout);
    return res;
}

static int check;
bool updater::exe(const QList<QString> &argv, const int time)
{
    QProcess process;
    QList<QString> list_argv;

    check = 1;
    connect(&process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
              [=](int exitCode, QProcess::ExitStatus){
        check = exitCode;
        qDebug() << "Exit Code "  << check << exitCode;
    });

    list_argv = QList<QString>("-Command") + argv;

    process.start("powershell", argv);
    process.waitForStarted(time);
    process.waitForFinished(time);
    qDebug() << check;
    return !check;
}

bool updater::createDirectory(const QString &path)
{
    QDir dir(path);
    if(dir.exists())
        return true;
    return dir.mkpath(path);
}

bool updater::removeDirectory(const QString &path)
{
    QDir directory(path);
    if(!directory.exists())
        return true;
    return directory.removeRecursively();
}

bool updater::cleanDirectory(const QString &path)
{
    QList<QString> argv;
    this->exe(QList<QString>(QString("mkdir %1").arg(path)), -1);
    argv << QString("cd %1; rm %2\\*").arg(path).arg(path);

    this->exe(argv, 10*1000);
    return true;
}

bool updater::moveWithA(const QString &from, const QString to)
{
    QList<QString> argv;

    if(!updater::cleanDirectory(to))
        return false;
    argv << QString("mv '%1' '%2'").arg(from).arg(to);

    qDebug() << argv;

    return this->exe(argv, 30*1000);
}

bool updater::removeFile(const QString &path)
{
    return QFile::remove(path);
}

#define pos_installation POS + "\\writernote.exe"
void updater::downloadUpdate()
{
    QDir __dir(POS);

    if(reply->error()){
        dialog_critic("We had this problem " + reply->errorString());
        delete manager;
        return;
    }

    const QByteArray &text = reply->readAll();
    const QJsonDocument doc = QJsonDocument::fromJson(text);
    const QString &dest_download = (QString)get_path(path::home) + "\\Downloads\\writernote_setup.zip";
    const QString &ver = doc[0]["name"].toString();
    const QString &url = "https://github.com/giacomogroppi/writernote-qt/releases/download/" + ver + "/writernote_win_setup_" + ver +".zip";
    const QString &dest_extraction = dest_download.mid(0, dest_download.indexOf(".zip"));

    if(!QFile::exists(POS)){
        if(!__dir.exists()){
            dialog_critic("I cant find writernote in " + POS);
            goto close;
        }
    }


    if(!downloadFile(url, dest_download)){
        dialog_critic("I had a problem downloading the file");
        goto close;
    }

    /* to run powershell as amministrator use:
        Start-Process powershell -Verb runAs

        we need this for move object into c:\programs
    */

    if(!extractFile(dest_download, dest_extraction)){
        dialog_critic("We had a problem extracting the zip into " + dest_extraction);
        goto close;
    }

    if(!moveWithA(dest_extraction, POS)){
        dialog_critic("We had a problem move writernote.exe to " + POS);
        goto close;
    }

    if(!areyousure("Remove file", "Do you want to remove the temporary file?")){
        goto close;
    }

    removeFile(dest_download);
    removeDirectory(dest_extraction);

    user_message("Writernote was updated successfully to version " + ver);

    close:
    this->close();
}

void updater::sslErrors(QNetworkReply *, const QList<QSslError> &errors)
{
    Q_UNUSED(errors);
    user_message("I can't check the current version becouse I had a problem\n");
}



