#include "mainwindow.h"
#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

#include "last_open/last_open.h"
#include "string.h"

#include "dataread/xmlstruct.h"
#include "utils/extract_audio/extract_audio.h"

#define HELP_COMMAND "\nTo extract an audio digit --extract, followed by the location of the file, the name of the copybook, \nand where you would like to save the audio\n\nTo open a file type the path of the file\n"
#define COMMAND_EXTRACT "--extract"

#ifdef CLOUD
#include "cloud/struct_user.h"
#include "cloud/cloud_controll.h"
#endif

static int extract_(const char *, const char *, const char *);

int main(int argc, char *argv[])
{
    if(argc == 5 && !strcmp(argv[1], COMMAND_EXTRACT)){
        return extract_(argv[2], argv[3], argv[4]);
    }

    if(argc != 1 && !strcmp(argv[1], "--help")){
        printf(HELP_COMMAND);
        return 0;
    }

    if(argc != 2 && argc != 1){
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
    TabletCanvas *canvas = new TabletCanvas;
    app->setCanvas(canvas);

    char * m_last_open = NULL;

    if(argc == 1){

#ifdef CLOUD
        last_open a(nullptr, user, &m_cloud, &close_all);
#else
        last_open a(nullptr, nullptr, nullptr, &close_all);
#endif
        a.setDataReturn(&m_last_open);

        if(a.load_data_() != 0){
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

#ifdef CLOUD
    MainWindow w(nullptr, &canvas, user, &m_cloud);
#else
    MainWindow w(nullptr, canvas);
#endif

    if(m_last_open != NULL)
        w.setFile(m_last_open);

    if(argc != 1){
        w.setFile(argv[1]);
    }

    w.show();
    int exit_code = app->exec();

#ifdef CLOUD
    if(user)
        delete user;
#endif

    return exit_code;
}

static int extract_(const char *f, const char *s, const char *t){
    auto res = extract_audio(f, s, t);

    if(res == extract::ok){
        printf("File extract correctly into %s", t);
        return 0;
    }
    if(res == extract::no_copybook){
        printf("There seems to be no copybook with this name inside the file\n");
        return -1;
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
        printf("Path to save [%s] is not valid\n", t);
        return -1;
    }
    printf("Unknown error\n");
    return -1;
}
