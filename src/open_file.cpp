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

static QByteArray get_path(const char *pos)
{
    QByteArray fileName;

    if(!pos){
        if(!qfilechoose::getFileForLoad(fileName, TYPEFILEWRITER | TYPEFILEPDF | TYPEALL)){
            return "";
        }
    }else{
        fileName = pos;
    }

    return fileName;
}

static bool checkExtention(const QString &fileName)
{
//#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
    // check if the file exist
    if(fileName.indexOf(APP_EXT) != -1 && fileName.indexOf(".pdf") != -1){
        if(!areyousure(QApplication::tr("Error"), QApplication::tr("The file does not have the writernote extension, or a pdf extention, do you want to open it anyway?"))){
            return false;
        }
    }
    return true;
//#endif
}

static bool exist(const QString &path)
{
    QFile file(path);
    if (!file.exists()){
        dialog_critic(QApplication::tr("I can't open the file because it doesn't exist"));
        return false;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        dialog_critic(QApplication::tr("I can't open this file because of the permission"));
        return false;
    }

    file.close();
    return true;
}

void MainWindow::openFile(const char *pos)
{
    QByteArray fileName, tmp;
    Document curr;
    xmlstruct xml(&fileName, _canvas->getDoc());
    n_need_save res_save;

    fileName = get_path(pos);

    if(fileName == "")
        return;

    if(!exist(fileName))
        return;

    if(!checkExtention(fileName))
        return;

    // restore lost file
    if(fileName.indexOf(".pdf") != -1 && fileLost::exe(fileName)){
        if(areyousure(QApplication::tr("Restore file"), QApplication::tr("Maybe the last time you opened this file the app closed suddenly, do you want to restore the file?"))){

            if(restore_file_critic::restore_file(fileName)){
                user_message(QApplication::tr("File restore correcty"));
            }else{
                return dialog_critic(QApplication::tr("If not restore correctly"));
            }
        }else{
            tmp = get_name_tmp::get(fileName);
            QFile::remove(tmp);
        }
    }


    // check if is pdf or we need to save the current document
    if(IS_PRESENT_IN_LIST(fileName, ".pdf")){
#ifdef PDFSUPPORT
        _canvas->getDoc()->m_pdf->addPdf(fileName, nullptr, this->m_path, *_canvas->getDoc()->datatouch);
        this->_preview_widget->changeDocument();
#else
        user_message("Pdf support is not enable in this version");
#endif // PDFSUPPORT
    }
    else {
        xml = xmlstruct(this->m_path, curr);
        res_save = this->needToSave(xml, curr);

        /*
         * in case there is already open a file, we need to controll if the
         * current file has beed modified
        */
        if(res_save == n_need_save::need_save){
            if(areyousure(QApplication::tr("Save need"), QApplication::tr("Do you want to save %1?").arg(this->m_path))){
                savefile save(&m_path, _canvas->getDoc());
                if(save.savefile_check_file(true) != OK){
                    if(!areyousure(QApplication::tr("Save fail"), QApplication::tr("We failed to save the file located in %1, should i continue?").arg(m_path))){
                        return;
                    }
                }
            }
        }

#if !(defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE))
        this->m_setting->changeCopybookFile();
#endif

        m_path = fileName;
        curr.reset();
        const auto res = xmlstruct(m_path, curr).loadfile(true, true);

        if(xmlstruct::manageMessage(res)){
            Document::copy(curr, *this->getCurrentDoc());

            _canvas->getDoc()->datatouch->triggerIfNone(-1);
            aggiornotestiriascolto(this);
            _canvas->updatePageCount();
            _canvas->getDoc()->datatouch->triggerVisibility(_canvas->height());
            _preview_widget->changeDocument();
            contrUi();
            _canvas->loadpixel();
            return;
        }

        this->m_path = "";
        this->_canvas->getDoc()->reset();

    }
    aggiornotestiriascolto(this);
    this->contrUi();
}

void MainWindow::on_actionOpen_triggered()
{
    openFile(NULL);
}
