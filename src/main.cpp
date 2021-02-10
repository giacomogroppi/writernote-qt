#include "mainwindow.h"

#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

#include "last_open/last_open.h"

int main(int argc, char *argv[])
{
    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    struct last_file *m_last_open = NULL;
    if(argc == 1){
        m_last_open = new struct last_file;

        last_open a(nullptr);

        a.setDataReturn(&last_file);

        if(a.load_data_() != 0){
            a.exec();
        }
    }

    //QApplication a(argc, argv);
    MainWindow w(nullptr, canvas);

    /*app.setApplicationName("writernote");
    app.setApplicationVersion("1.3.12");
    app.setOrganizationDomain("https://github.com/giacomogroppi/writernote-qt");
    app.setOrganizationName("writernote");*/

    if(m_last_open != NULL)
        w.setFile(m_last_open->posizione);

    if(argc != 1){
        w.setFile(argv[1]);
    }

    w.show();
    return app.exec();

}
