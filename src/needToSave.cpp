#include "dataread/xmlstruct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "currenttitle/checksimilecopybook.h"

enum MainWindow::n_need_save
        MainWindow::needToSave(
            xmlstruct *xml,
            currenttitle_class * tmp_read,
            indice_class *tmp_ind) const{
    QString filep;
    int check1;

    check1 = xml->loadfile((m_currentTitle + ".xml").toUtf8().constData());

    if(check1 != ERROR_VERSION_NEW){
        if(check1 != OK){
            return n_need_save::unable_load;
        }
    }

    if(!xml->loadindice()){
        return n_need_save::unable_load;
    }


    /*
     * if we are in keyboard mode
    */
    if(!m_currenttitle->m_touch){
        filep = m_currenttitle->testi;
        m_currenttitle->testi = this->ui->textEdit->toHtml();
    }

    check1 = checksimilecopybook(tmp_read, m_currenttitle, true) == OK_CHECK;

    /* if we pass nullptr */
    if(tmp_ind){
        check1 = check1 && checksimileindice(&m_indice, tmp_ind) == OK_CHECK;
    }

    m_currenttitle->testi = filep;

    if(check1)
        return n_need_save::not_;

    if(m_currenttitle->m_touch){
        if(!m_currenttitle->datatouch->userWrittenSomething(tmp_read->datatouch)){
            return n_need_save::only_writernote;
        }
    }

    return n_need_save::need_save;

}
