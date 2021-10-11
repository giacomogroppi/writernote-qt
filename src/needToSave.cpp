#include "dataread/xmlstruct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "currenttitle/checksimilecopybook.h"

/*
 * tmp_read and tmp_ind
 * they are temporary objects, which are used by him to
 * understand the file currently used, that of MainWindow,
 *  has been modified
*/

enum MainWindow::n_need_save
        MainWindow::needToSave(
            xmlstruct & xml,
            Document & tmp_read) const{
    int check1;

    if(xml.getPath() == ""){
        if(!m_currenttitle->datatouch->userWrittenSomething(nullptr))
            return n_need_save::only_writernote;
        return n_need_save::no_path;
    }

    check1 = xml.loadfile(false, false);

    if(check1 != ERROR_VERSION_NEW &&
            check1 != OK &&
            check1 != ERROR_CONTROLL){
        return n_need_save::unable_load;
    }

    if(!m_currenttitle->datatouch->userWrittenSomething(tmp_read.datatouch)){
        return n_need_save::only_writernote;
    }

    check1 = checksimilecopybook(tmp_read, *m_currenttitle, true) == OK_CHECK;

    if(check1)
        return n_need_save::not_;

    if(this->m_currenttitle->isEmpty() || m_currenttitle->datatouch->userWrittenSomething(nullptr)){
        return n_need_save::not_;
    }

    return n_need_save::need_save;

}
