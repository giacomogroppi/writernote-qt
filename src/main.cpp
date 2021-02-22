#include "mainwindow.h"

#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

#include "last_open/last_open.h"

#include "cloud/struct_user.h"

int main(int argc, char *argv[])
{
#ifdef CLOUD
    struct struct_user *user;
    user = load_recent_user();
#endif

    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    char * m_last_open = NULL;
    if(argc == 1){
        last_open a(nullptr);

        a.setDataReturn(&m_last_open);

        if(a.load_data_() != 0){
            a.exec();
        }
    }

    //QApplication a(argc, argv);

#ifdef CLOUD
    MainWindow w(nullptr, canvas, user);
#else
    MainWindow w(nullptr, canvas);
#endif

    if(m_last_open != NULL)
        w.setFile(m_last_open);

    if(argc != 1){
        w.setFile(argv[1]);
    }

    w.show();
    int exit_code = app.exec();

#ifdef CLOUD
    if(user)
        delete user;
#endif

    return exit_code;
}
