#include "mainwindow.h"
#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"
#include "last_open/last_open.h"
#include "string.h"
#include "dataread/xmlstruct.h"
#include "ui/uicore.h"
#include "utils/extract_audio/extract_audio.h"
#include "utils/changeLanguage.h"
#include "log/log_ui/log_ui.h"
#include "testing/testingcore.h"
#include "utils/common_error_definition.h"
#include "testing/memtest.h"
#include "core/core.h"
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
static not_used void createFileAndExit(const QString &path, MainWindow *parent);

#ifdef DEBUG_CORE
static void manageDebug(MainWindow *parent)
{
    TestingCore core(parent);
    exit(core.startTesting());
}
#else
# define manageDebug(x) ;
#endif

int main(int argc, char **argv)
{
    TabletApplication *app;
    TabletCanvas *canvas;
    MainWindow *window;
    char * m_last_open = NULL;
    int exit_code;

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

    /*{
        QImage img;
        QPainter painter;
        Define_PEN(pen);

        cint minAngle = 270  * 16;
        cint maxAngle = 90 * 16;
        cint deltaBorder = 10;
        cint width = 2000;
        cint height = 2000;
        cint startCircle = 550;

        img = QImage(width * 2, height * 2, QImage::Format_ARGB32);

        painter.fillRect(img.rect(), Qt::white);

        painter.begin(&img);

        pen.setWidth(50);
        pen.setColor(Qt::blue);

        painter.setPen(pen);

        painter.drawLine(deltaBorder,           deltaBorder,
                         width + deltaBorder,   deltaBorder);

        painter.drawLine(deltaBorder, deltaBorder,
                         deltaBorder, deltaBorder + height);

        painter.drawLine(deltaBorder,                       deltaBorder + height,
                         deltaBorder + width - startCircle, deltaBorder + height);

        painter.drawLine(width + deltaBorder, deltaBorder,
                         width + deltaBorder, deltaBorder + height - startCircle);

        cint del = 550;

        painter.drawArc(QRect(
                            deltaBorder + width - startCircle - del, deltaBorder + height - startCircle - del,
                            startCircle * 2, startCircle * 2
                            ), minAngle, maxAngle);

        pen.setWidth(200);
        painter.setPen(pen);

        cint delta = 550;

        painter.drawPoint(width / 2, (height / 10) * 2);
        painter.drawPoint(width / 2, (height / 10) * 2 + delta);
        painter.drawPoint(width / 2, (height / 10) * 2 + delta * 2);


        painter.end();
        img.save("/home/giacomo/img_writernote.png", "PNG");

        system("xdg-open /home/giacomo/img_writernote.png");

        return 0;
    }*/

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

    manageDebug(&w);

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
