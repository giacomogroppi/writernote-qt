#include "mainwindow.h"
#include <QMimeData>
#include "utils/dialog_critic/dialog_critic.h"
#include "utils/common_def.h"
#include "images/fromimage.h"
#include <QApplication>
#include "core/WFile.h"
#define WRITERNOTE 1
#define IMAGE 2

QList<QString> drop_event_get_list(const QMimeData *event)
{
    W_ASSERT(event);
    QList<QString> to;

    if(!event->hasUrls())
        return {};

    const auto &url = event->urls();

    for(const auto &p : qAsConst(url)){
        const auto &path_tmp = p.toLocalFile();

        W_ASSERT(WFile::fileExist(path_tmp.toUtf8()) == 0);
        to.append(path_tmp);
    }

    return to;
}

static int mainwindow_import_image(const QList<QString> &Path, Document &doc,
                                   const QPointF &point, const QByteArray &zip_pos)
{
    PointSettable s = {
        point,
        true
    };

    if(WFile::fileExist(zip_pos) < 0)
        return -1;

    for(const auto &path : qAsConst(Path)){
        const auto res = doc.m_img->addImage(path, &s, zip_pos);
        if(unlikely(res < 0))
            return -1;
    }

    return 0;
}

static void show_error_extentions()
{
    dialog_critic(QApplication::tr("You have selected at least two object with different extentions"));
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString path_to_load;
    int find = 0;
    int i;
    QImage image;
    PointSettable point;
    const auto positions = drop_event_get_list(event->mimeData());

    for(const auto &path : qAsConst(positions)){
        if(path.indexOf("." APP_EXT) != -1){
            if(find and find != WRITERNOTE)
                return show_error_extentions();

            path_to_load = path;
            find = WRITERNOTE;
        }else if(image.load(path)){
            if(find and find != IMAGE)
                // we find some object with different extentions
                return show_error_extentions();

            find = IMAGE;
        }
    }

    if(find == WRITERNOTE){
        if(this->m_path != path_to_load){
            openFile(path_to_load.toUtf8().constData());
        }
    }else if(find == IMAGE){
        point = event->position();
        const auto res = mainwindow_import_image(positions, *_canvas->data, event->position(), m_path);
        //const auto res = this->_canvas->data->m_img->addImage(path_to_load, &point, this->m_path);
        if(res < 0)
            dialog_critic(QApplication::tr("We got some problem importing the image"));
    }
    else{
        user_message("The file you are trying to open does not have a compatible extention");
    }
    _canvas->call_update();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}
