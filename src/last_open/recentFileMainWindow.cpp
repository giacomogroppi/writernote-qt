#include "mainwindow.h"
#include "last_open.h"

static last_open *__last = nullptr;
void MainWindow::on_actionRecent_file_triggered()
{
    char *data = nullptr;

    if(!__last){
        __last = new last_open(this, nullptr, nullptr, nullptr, last_open::Method::OpenFile);
    }
    __last->setDataReturn(&data);
    __last->load_data_();
    __last->exec();

    if(data){
        this->openFile(data);
        delete [] data;
    }
}
