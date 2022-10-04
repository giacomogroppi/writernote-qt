#include "mainwindow.h"
#include "reduce_size.h"
#include "utils/areyousure/areyousure.h"
#include "utils/dialog_critic/dialog_critic.h"


void MainWindow::on_actionReduce_size_file_triggered()
{
    if(this->m_path == "")
        return;

    size_t __c, __e;

    __c = reduce_size::current_size(_canvas->getDoc()->datatouch);
    __e = reduce_size::calculate_min_size(_canvas->getDoc()->datatouch);

    if(areyousure("Reduce file writernote",
                  "With writernote you can decide to decrease the size of the files, decreasing the precision of the strokes you draw\nThe current size is "
                  + QString::number(__c) + " byte"
                  + " the min size is "
                  + QString::number(__e) + " byte"
                  + "\nDo you want to decrese the information?")){

        reduce_size::decrese(_canvas->getDoc()->datatouch);

        __c = reduce_size::current_size(_canvas->getDoc()->datatouch);
        user_message("The current size is now "
                        + QString::number(__c) + " byte");

    }
}
