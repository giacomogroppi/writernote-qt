#include "mainwindow.h"

#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

#include "last_open/last_open.h"

#include "cloud/struct_user.h"

int main(int argc, char *argv[])
{
    struct struct_user *user;
    user = load_recent_user();


    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    last_file *m_last_open = NULL;
    if(argc == 1){
        last_open a(nullptr);

        a.setDataReturn(&m_last_open);

        if(a.load_data_() != 0){
            a.exec();
        }
    }

    //QApplication a(argc, argv);
    MainWindow w(nullptr, canvas);

    if(m_last_open != NULL)
        w.setFile(m_last_open->posizione);

    if(argc != 1){
        w.setFile(argv[1]);
    }

    w.show();
    int exit_code = app.exec();

    if(user)
        delete user;

    return exit_code;
}
