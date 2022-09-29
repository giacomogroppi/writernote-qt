#include "mainwindow.h"
#include "qfilechoose.h"
#include "ui_mainwindow.h"
#include "savefile.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/common_script.h"

static inline const QString move_file(const QString &nomevecchio, const QString &nomenuovo)
{
    return ("cp " + nomevecchio + " " + nomenuovo);
}

static bool c = true;

static int has_extensions(const QString &str)
{
    constexpr int l = WStrlen("." APP_EXT);
    const int len = str.length();
    if(len < l){
        return -1;
    }

    const auto s = str.mid(len - l, l);
    const auto ext_str = qstr("." APP_EXT);
    if (s != ext_str)
        return -1;
    return 0;
}

#ifdef SNAP
static int adjust_extensions(const QByteArray &str)
{
    if(has_extensions(str) < 0){
        dialog_critic(QApplication::tr(qstr("The file must have the extension %1").arg(ext).toUtf8().constData()));
        return -1;
    }
    return 0;
}
#else
static int adjust_extensions(QByteArray &str)
{
    if(has_extensions(str) < 0){
        str.append("." APP_EXT);
        W_ASSERT(has_extensions(str));
    }
    return 0;
}
#endif

/* funzione che viene richiamata quando viene salvato */
void MainWindow::on_actionSave_File_triggered()
{
    savefile savefile_i(&m_path, _canvas->data);
    QByteArray new_path;
    bool check;

    if(m_path == ""){
        if(!qfilechoose::getFileForSave(new_path, TYPEFILEWRITER)){
            return;
        }
        m_path = new_path;
    }

    if(adjust_extensions(m_path) < 0)
        return;

    check = (savefile_i.savefile_check_file(true) == OK);

    if(!check && c)
        return dialog_critic("We had a problem while saving the file");

    c = true;
}

void MainWindow::on_actionSave_As_triggered()
{
    auto posizione = this->m_path;

    if(!qfilechoose::getFileForSave(m_path))
        /* se l'utente non ha selezionato nessun file */
       return;

    if(posizione != ""){
        int command;

        command = system(move_file(posizione, m_path).toUtf8().constData());

        if(command){
            dialog_critic("We had a problem saving a copy of " + posizione + " to " + m_path);
            m_path = posizione;
            return;
        }

        c = false;

        /*save the file in the new path -> keeping the current file the same*/
        this->on_actionSave_File_triggered();
        if(c){
            dialog_critic("We had a problem saving the copybook in " + m_path);
        }
        m_path = posizione;
    }
    else
        return this->on_actionSave_File_triggered();

}
