#include "dialog_critic.h"

#include <QMessageBox>

void dialog_critic(const QString &str){
    QMessageBox msgBox;
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    msgBox.setWindowState(Qt::WindowFullScreen);
#endif // ANDROID IOS_WRITERNOTE
    msgBox.setText(str);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void user_message(const QString &str){
    QMessageBox msgBox;
#if defined(ANDROID_WRITERNOTE) || defined(IOS_WRITERNOTE)
    msgBox.setWindowState(Qt::WindowFullScreen);
#endif // ANDROID IOS_WRITERNOTE
    msgBox.setText(str);
    msgBox.exec();
}

