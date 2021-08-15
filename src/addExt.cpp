#include "mainwindow.h"
#include "frompdf/frompdf.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "images/fromimage.h"

uchar MainWindow::addPdf(const QString &path){
    /*
     * TODO: se il file che si sta modificando non è ancora stato salvato
     * non chiedere all'utente di salvare il file, ma salvarlo solamente
     * quando si salva il file, quindi mettere una variabile che viene settata
     * quando è necessario salvare il file
    */
    frompdf::load_res res;

    if(!this->m_currenttitle->m_touch){
        dialog_critic("Pdf annotation and reading is not support in keyboard mode");
        return ERROR;
    }

    if(m_path == ""){
        dialog_critic("Before use pdf it's necessery to save the current file");
        return ERROR;
    }

    m_currenttitle->count_pdf ++;

    res = this->m_currenttitle->m_pdf->save(nullptr, path,
                                      this->m_path);

    if(res != frompdf::load_res::ok){
        dialog_critic("We had a problem saving " + path + " into " + m_path);
        m_currenttitle->count_pdf --;
        return ERROR;
    }

    return OK;
}
