#include "mainwindow.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/areyousure/areyousure.h"
#include "dataread/xmlstruct.h"
#include "utils/common_def.h"
#include "ui_mainwindow.h"
#include "filelost.h"
#include "restore_file/get_name_tmp.h"
#include "restore_file/ui/setting_restore_ui.h"
#include "restore_file/restore_file_critic.h"
#include "frompdf/frompdf.h"
#include "last_open/last_open.h"
#include "datawrite/savefile.h"
#include "datawrite/qfilechoose.h"
#include "audioplay/aggiornotastiriascolto.h"
#include "utils/common_script.h"
#include "preview_page_widget/preview_page_widget.h"
#include <QFile>
#include <QFileDialog>

void MainWindow::openFile(const char *pos)
{
    QString fileName, tmp;
    Document curr;
    xmlstruct xml(&fileName, _canvas->data);
    n_need_save res_save;

    if(!pos){
        if(!qfilechoose::getFileForLoad(fileName, TYPEFILEWRITER | TYPEFILEPDF | TYPEALL)){
            return;
        }
        //fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home/", "Writernote (*." + APP_EXT + ");; Pdf (*.pdf);; All file (* *.*)");
    }else{
        fileName = pos;
    }

    if(fileName == "")
        return;

    QFile file(fileName);
    if (!file.exists())
        return dialog_critic("I can't open the file because it doesn't exist");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return dialog_critic("I can't open this file because of the permission");

    file.close();

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    // check if the file exist
    if(fileName.indexOf(APP_EXT) != -1
            && fileName.indexOf(".pdf") != -1){
        if(!areyousure("Error", "The file does not have the writernote extension, or a pdf extention, do you want to open it anyway?")){
            return;
        }
    }
#endif

    // restore lost file
    if(fileName.indexOf(".pdf") != -1 && fileLost::exe(fileName)){
        if(areyousure("Restore file", "Maybe the last time you opened this file the app closed suddenly, do you want to restore the file?")){

            if(restore_file_critic::restore_file(fileName)){
                user_message("File restore correcty");
            }else{
                return dialog_critic("If not restore correctly");
            }
        }else{
            tmp = get_name_tmp::get(fileName);
            QFile::remove(tmp);
        }
    }


    // check if is pdf or we need to save the current document
    if(IS_PRESENT_IN_LIST(fileName, ".pdf")){
#ifdef PDFSUPPORT
        _canvas->data->m_pdf->addPdf(fileName, nullptr, this->m_path, this->_canvas);
#else
        user_message("Pdf support is not enable in this version");
#endif // PDFSUPPORT
    }
    else {
        res_save = this->needToSave(xml, curr);

        /*
         * in case there is already open a file, we need to controll if the
         * current file has beed modified
        */
        if(res_save == n_need_save::need_save){
            if(areyousure("Save need", "Do you want to save " + this->m_path + "?")){
                savefile save(&m_path, _canvas->data);
                if(save.savefile_check_file(true) != OK){
                    if(!areyousure("Save fail", "We failed to save the file, should i continue?")){
                        return;
                    }
                }
            }
        }

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
        this->m_setting->changeCopybookFile();
#endif

        m_path = fileName;
        const auto res = xml.loadfile(true, true);

        if(xmlstruct::manageMessage(res)){
            _canvas->data->datatouch->triggerIfNone(-1);
            //this->_canvas->data->datatouch->triggerNewView(-1, true);
            aggiornotestiriascolto(this);
            _preview_widget->changeDocument();
            _canvas->updatePageCount();
            _canvas->data->datatouch->triggerVisibility(_canvas->height());
            contrUi();
            _canvas->loadpixel();
            return;
        }

        this->m_path = "";
        this->_canvas->data->reset();

    }
    aggiornotestiriascolto(this);
    this->contrUi();
}

void MainWindow::on_actionOpen_triggered()
{
    openFile(NULL);
}
