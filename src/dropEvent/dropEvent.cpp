#include "../mainwindow.h"
#include <QMimeData>
#include "../utils/dialog_critic/dialog_critic.h"

/*
 * return 1 if he can accent
*/
static uchar IsAvailable(const QString &);

void MainWindow::dropEvent(QDropEvent *event){
    const QMimeData* mimeData = event->mimeData();

    QString __path_to_load;
    uchar find = 0;

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
         QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files
        for (int i = 0; i < urlList.size() && i < 32 && !find; ++i)
        {
            if(IsAvailable(urlList.at(i).toLocalFile())){
                __path_to_load = urlList.at(i).toLocalFile();
                find = 1;
            }
        }
    }

    if(find == 1){
        if(this->m_path != __path_to_load)
            this->on_actionOpen_triggered(__path_to_load.toUtf8().constData());
    }else{
        messaggio_utente("The file you are trying to open does not have the file writernote extension");
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}

static uchar IsAvailable(const QString &pos){
    return pos.indexOf(".writer") != -1;
}
