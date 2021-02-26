#include "mainwindow.h"

#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

#include "last_open/last_open.h"

#include "cloud/struct_user.h"
#include "cloud/cloud_controll.h"

int main(int argc, char *argv[])
{
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
    last_open *a = NULL;

    if(argc == 1){

#ifdef CLOUD
        a = new last_open(nullptr, user, &m_cloud, &close_all);
#else
        last_open a(nullptr);
#endif
        a->setDataReturn(&m_last_open);

        if(a->load_data_() != 0){
            a->exec();
        }
    }

    if(a)
        delete a;

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
    MainWindow w(nullptr, canvas, user, &m_cloud);
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
