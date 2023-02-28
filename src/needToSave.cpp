#include "dataread/xmlstruct.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "currenttitle/checksimilecopybook.h"
#include "utils/common_error_definition.h"

/*
 * tmp_read and tmp_ind
 * they are temporary objects, which are used by him to
 * understand the file currently used, that of MainWindow,
 *  has been modified
*/

enum MainWindow::n_need_save
        MainWindow::needToSave(
            xmlstruct   &xml,
            Document    &tmp_read) const
{
    const auto debugNeedSave = true;
    int check1;
    Document *doc = _canvas->getDoc();

    if(xml.getPath() == ""){
        WDebug(debugNeedSave, "Path empty");
        if(doc->userWrittenSomething()){
            WDebug(debugNeedSave, "Need to save [1]");
            return n_need_save::need_save;
        }
        WDebug(debugNeedSave, "Don't need to save");
        return n_need_save::no;
    }else{
        WDebug(debugNeedSave, qstr("Check file in %1").arg(xml.getPath()));
    }

    check1 = xml.loadfile(false, false);

    if(check1 != ERROR_VERSION_NEW and check1 != OK and check1 != ERROR_CONTROLL){
        WDebug(debugNeedSave, qstr("Not possibile to load file in %1").arg(xml.getPath()));
        return n_need_save::unable_load;
    }

    if(!datastruct::userWrittenSomething(*doc, tmp_read)){
        WDebug(debugNeedSave, "Don't need to save [2]");
        return n_need_save::no;
    }

    /*check1 = checksimilecopybook(tmp_read, *doc) == OK_CHECK;

    if(check1)
        return n_need_save::no;

    if(doc->isEmpty() || doc->datatouch->userWrittenSomething(nullptr)){
        return n_need_save::no;
    }*/

    WDebug(debugNeedSave, "Need to save [2]");

    return n_need_save::need_save;

}
