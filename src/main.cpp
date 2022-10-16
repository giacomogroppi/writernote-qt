#include "mainwindow.h"
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"
#include "last_open/last_open.h"
#include "ui/uicore.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/extract_audio/extract_audio.h"
#include "utils/changeLanguage.h"
#include "log/log_ui/log_ui.h"
#include "utils/common_error_definition.h"
#include "testing/memtest.h"
#include "core/core.h"
#include "ui/WStyle.h"
#include "utils/common_script.h"
#include <QStyleFactory>
#include <QFile>

#define HELP_COMMAND "\nTo extract an audio digit --extract, followed by the location of the file\nand where you would like to save the audio\n\nTo open a file type the path of the file\n"
#define COMMAND_EXTRACT "--extract"

#ifdef CLOUD
#include "cloud/struct_user.h"
#include "cloud/cloud_controll.h"
#endif

static void printLog();
static int extract_(const char *path, const char *path_to);

#if defined(DEBUGCORE)
static not_used void createFileAndExit(const QString &path, MainWindow *parent);
#else
static not_used void createFileAndExit(const QString &, MainWindow *) { };
#endif

#ifdef DEBUG_CORE
# define test() return 0;
static void manageDebug(MainWindow *parent)
{
    //TestingCore core(parent);
    //exit(core.startTesting());
}
#else
# define test() ;
# define manageDebug(x) ;
#endif

int main(int argc, char **argv)
{
    TabletApplication *app;
    TabletCanvas *canvas;
    MainWindow *window;
    char * m_last_open = nullptr;
    int exit_code;

    test();

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

#if defined(WIN32) || defined(WIN64)
    QApplication::setStyle("fusion");
#endif

    WNew(app, TabletApplication, (argc, argv));

    app->setPalette(WStyle::get_palette());

    language_manager::setLanguage(app);

    WNew(canvas, TabletCanvas, ());

    app->setCanvas(canvas);

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
    WNew(window, MainWindow, (nullptr, canvas, user, &m_cloud, f));
#else
    WNew(window, MainWindow, (canvas, nullptr, nullptr, f));
#endif    

    if(core::is_dark_mode()){
        UiCore::adjustDarkMode();
    }

    qDebug() << "is dark mode?: " << core::is_dark_mode();
    qDebug() << "Is running on wayland?" << is_wayland();

    manageDebug(window);

    QList<Qt::GestureType> gestures;
    gestures << Qt::PanGesture;
    gestures << Qt::PinchGesture;
    gestures << Qt::SwipeGesture;

    window->grabGestures(gestures);

    window->show();
    exit_code = app->exec();

#ifdef CLOUD
    if(user)
        delete user;
#endif

    printLog();

    WDelete(canvas);
    WDelete(window);
    WDelete(app);

    WEnd_application();

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
    if(unlikely(!NAME_LOG_EXT)){
        printf("No log available\n");
        return;
    }
    if(unlikely(!NAME_LOG_EXT->getData(arr))){
        //printf("Error open file log in %s\n", NAME_LOG_EXT->getCurrentPosition().toUtf8().constData());
        return;
    }

    printf("Log:\n");
#ifdef DEBUGINFO
    dialog_critic(arr);
    QFile::remove(NAME_LOG_EXT->getCurrentPosition());
#else
    NAME_LOG_EXT->print(stdin, arr);
#endif
}

#ifdef DEBUGCORE

static not_used void createFileAndExit(const QString &path, MainWindow *parent)
{
    TestingCore core(parent);
    int err = core.createAndSave(path, NULL);
    if(err != OK){
        qDebug() << "Error save" << err;
    }

    exit(0);
}

#endif

#ifdef DEBUGINFO
#pragma message ("Debug build")
#else
#pragma message ("Release build")
#endif

static_assert(WStrEqual(APP_EXT, "writer") == true);
static_assert(sizeof(int) == 4);
static_assert(sizeof(double) == 8);
static_assert(sizeof(pressure_t) == 4);
static_assert(sizeof(size_t) == 8);
