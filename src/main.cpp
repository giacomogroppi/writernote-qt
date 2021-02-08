#include "mainwindow.h"

#include <QApplication>
#include "touch/tabletapplication.h"
#include "touch/tabletcanvas.h"

int main(int argc, char *argv[])
{
    TabletApplication app(argc, argv);
    TabletCanvas *canvas = new TabletCanvas;
    app.setCanvas(canvas);

    //QApplication a(argc, argv);
    MainWindow w(nullptr, canvas);

    /*app.setApplicationName("writernote");
    app.setApplicationVersion("1.3.12");
    app.setOrganizationDomain("https://github.com/giacomogroppi/writernote-qt");
    app.setOrganizationName("writernote");*/


    if(argc != 1){
        w.setFile(argv);
    }

    w.show();
    return app.exec();

}
