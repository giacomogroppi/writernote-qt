#include "mainwindow.h"
#include <QMimeData>
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/common_def.h"
#include "images/fromimage.h"
#include <QApplication>
#define WRITERNOTE 1
#define IMAGE 2


void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData* mimeData = event->mimeData();

    QString __path_to_load;
    uchar find = 0;
    uint i;
    QImage image;
    PointSettable point;

    QList<QUrl> urlList;

    if (!mimeData->hasUrls())
        return;

    urlList = mimeData->urls();

    for(i = 0; i < (uint)urlList.size() && i < 32 && !find; ++i){
        const QString & __path = urlList.at(i).toLocalFile();

        if(__path.indexOf("." + APP_EXT) != -1){
            __path_to_load = __path;
            find = WRITERNOTE;
        }else if(image.load(__path)){
            find = IMAGE;
        }
    }

    if(find == WRITERNOTE){
        if(this->m_path != __path_to_load){
            openFile(__path_to_load.toUtf8().constData());
        }
    }else if(find == IMAGE){
        point.point = event->posF();

        const auto res = this->_canvas->data->m_img->addImage(__path_to_load, &point, this->m_path);
        if(res < 0)
            dialog_critic(QApplication::tr("We got some problem importing the image"));
    }
    else{
        user_message("The file you are trying to open does not have a compatible extention");
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    event->acceptProposedAction();
}
