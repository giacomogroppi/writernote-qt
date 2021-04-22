#include "dataread/xmlstruct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "currenttitle/checksimilecopybook.h"

enum MainWindow::n_need_save
        MainWindow::needToSave(
            xmlstruct *xml,
            currenttitle_class * tmp_read,
            indice_class *tmp_ind) const{
    QString filep, __message ;
    bool check1;


    if(m_currenttitle->isEmpty())
        return n_need_save::not_;


    if(xml->loadfile((m_currentTitle + ".xml").toUtf8().constData()) != OK
            || !xml->loadindice()){
        return n_need_save::unable_load;
    }


    /*
     * if we are in keyboard mode
    */
    if(!m_currenttitle->m_touch){
        filep = m_currenttitle->testi;
        m_currenttitle->testi = this->ui->textEdit->toHtml();
    }

    check1 = checksimilecopybook(tmp_read, m_currenttitle, false) == OK_CHECK
            && checksimileindice(&m_indice, tmp_ind) == OK_CHECK;

    if(!check1){

        if(m_currenttitle->m_touch){
            if(!m_currenttitle->datatouch->userWrittenSomething()
                    && !tmp_read->datatouch->userWrittenSomething()){
                return n_need_save::only_writernote;
            }
        }

        return n_need_save::need_save;
    }


    return n_need_save::need_save;
}
