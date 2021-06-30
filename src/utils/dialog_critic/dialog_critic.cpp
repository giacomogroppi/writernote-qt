#include "dialog_critic.h"

#include <QMessageBox>

void dialog_critic(const QString &str){
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void user_message(const QString &str){
    QMessageBox msgBox;
    msgBox.setText(str);
    msgBox.exec();
}

