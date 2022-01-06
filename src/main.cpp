#include "mainwindow.h"
#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"
#include "last_open/last_open.h"
#include "string.h"
#include "dataread/xmlstruct.h"
#include "utils/extract_audio/extract_audio.h"
#include "utils/changeLanguage.h"
#include "log/log_ui/log_ui.h"
#include "testing/testingcore.h"
#include "utils/common_error_definition.h"

#define HELP_COMMAND "\nTo extract an audio digit --extract, followed by the location of the file\nand where you would like to save the audio\n\nTo open a file type the path of the file\n"
#define COMMAND_EXTRACT "--extract"

#ifdef CLOUD
#include "cloud/struct_user.h"
#include "cloud/cloud_controll.h"
#endif

static void printLog();
static int extract_(const char *path, const char *path_to);
static void createFileAndExit(const QString &path, MainWindow *parent);

#ifdef DEBUG_THREAD
static void manageDebug(MainWindow *parent)
{
    TestingCore core(parent);
    exit(core.startTesting());
}
#else
#define manageDebug(x) ;
#endif

int main(int argc, char *argv[]){
    if(argc == 5 && !strcmp(argv[1], COMMAND_EXTRACT)){
        return extract_(argv[2], argv[3]);
    }

    if(argc != 1 && !strcmp(argv[1], "--help")){
        printf(HELP_COMMAND);
        return 0;
    }

    if(argc > 2){
        printf("Error");

        printf(HELP_COMMAND);

        return -1;
    }

#ifdef CLOUD
    struct struct_user *user;
    user = load_recent_user();

    cloud_controll m_cloud(user);

#endif

    bool close_all = false;
    TabletApplication *app = new TabletApplication(argc, argv);

    language_manager::setLanguage(app);

    TabletCanvas *canvas = new TabletCanvas;
    app->setCanvas(canvas);

    char * m_last_open = NULL;

    if(argc == 1){

#ifdef CLOUD
        last_open a(nullptr, user, &m_cloud, &close_all, last_open::Method::OpenRecent);
#else
        last_open a(nullptr, nullptr, nullptr, &close_all, last_open::Method::OpenRecent);
#endif
        a.setDataReturn(&m_last_open);

        if(a.load_data_() > 0){
            a.exec();
        }
    }


    /*
     * set by load_open to true if the user want to quit all
    */
    if(close_all){
#ifdef CLOUD
        if(user)
            delete user;
#endif
        return 0;
    }

    char *f = (m_last_open) ? m_last_open : argv[1];

#ifdef CLOUD
    MainWindow w(nullptr, canvas, user, &m_cloud, f);
#else
    MainWindow w(nullptr, canvas, nullptr, nullptr, f);
#endif

    //if(m_last_open != NULL)
    //    w.setFile(m_last_open);

    //delete m_last_open;

    //if(argc != 1){
    //    w.setFile(argv[1]);
    //}

    manageDebug(&w);

    //createFileAndExit("/home/giacomo/writernote-qt/test/file_test.writer", &w);

    w.show();
    int exit_code = app->exec();

#ifdef CLOUD
    if(user)
        delete user;
#endif

    printLog();

    delete canvas;
    return exit_code;
}

static int extract_(const char *path, const char *path_to){
    auto res = extract_audio(path, path_to);

    if(res == extract::ok){
        printf("File extract correctly into %s", path_to);
        return 0;
    }
    if(res == extract::load_file){
        printf("We had a problem uploading the file\n");
        return -1;
    }
    if(res == extract::not_record){
        printf("The copybook you entered has not been registered\n");
        return -1;
    }
    if(res == extract::load_audio){
        printf("We had a problem reading the audio\n");
        return -1;
    }
    if(res == extract::open_to){
        printf("Path to save [%s] is not valid\n", path_to);
        return -1;
    }
    printf("Unknown error\n");
    return -1;
}

static void printLog()
{
    QByteArray arr;
    if(!NAME_LOG_EXT){
        printf("No log available\n");
        return;
    }
    if(!NAME_LOG_EXT->getData(arr)){
        printf("Error open file log in %s\n", NAME_LOG_EXT->getCurrentPosition().toUtf8().constData());
        return;
    }

    printf("Log:\n");
    NAME_LOG_EXT->print(stdin, arr);
}

__attribute__((unused)) static void createFileAndExit(const QString &path, MainWindow *parent)
{
    TestingCore core(parent);
    int err = core.createAndSave(path, NULL);
    if(err != OK){
        qDebug() << "Error save" << err;
    }

    exit(0);
}
