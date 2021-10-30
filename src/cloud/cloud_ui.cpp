#include "cloud_ui.h"
#include "ui_cloud_ui.h"
#include <QMessageBox>
#include "../windows/mostra_finestra_i.h"
#include "../utils/dialog_critic/dialog_critic.h"
#include "../utils/checkpassword/checkpassword.h"

cloud_ui::cloud_ui(QDialog *parent, cloud_controll *cloud) :
    QDialog(parent),
    m_controll(cloud),
    ui(new Ui::cloud_ui)
{
    ui->setupUi(this);

    this->setWindowTitle(NAME);

    this->refresh();

    ui->edit_repeat->setHidden(true);
    ui->label_repeat->setHidden(true);

    /*
     * when the server replied to our request, we connect
     * the cloud controll function, which will be issued
     * by void readyread () and analyze the data
    */
    QObject::connect(cloud, &cloud_controll::readyReadExt, this, &cloud_ui::readyRead);
}

cloud_ui::~cloud_ui()
{
    delete ui;
}

void cloud_ui::readyRead(QByteArray data, n_request::e_request last_command){
    const char *temp = data.data();
    int return_command;
    memcpy(&return_command, temp, sizeof(int));

    if(last_command == n_request::login_user){
        if(return_command == n_error_cloud::password_wrong){
            user_message("Password wrong");
        }else if(return_command == n_error_cloud::ok){
            save_recent_user(m_controll->m_user);
        }
    }else if(last_command == n_request::register_user){
        if(return_command == n_error_cloud::server_down)
            return serverDown();

        if(return_command == n_error_cloud::ok){
            user_message("Successful registration");
        }else{
            user_message("Something went wrong");
        }
    }
}

void cloud_ui::serverDown(){
    user_message("Unfortunately the server is not reachable, please try again in some time");
}

void cloud_ui::on_button_info_clicked()
{
    /*
     * info
    */
    QMessageBox msgBox;
    msgBox.setWindowTitle("Info cloud");
    msgBox.setText("Writernote has decided to start an intelligent cloud system,\nto give you the possibility to save the files you write in the cloud. You can share files with your contacts, and decide what they can do with them, whether to give them only permission to edit them, or just read them.\nYou can also read your notes in the cloud from any device, be it windows, android, or linux.");
    msgBox.exec();
}

void cloud_ui::on_close_button_clicked()
{
    this->close();
}

#define WEB1 "https://writernote-b.ddns.net/1_m"
void cloud_ui::on_ac_1_clicked()
{
    mostra_finestra_i(WEB1);
}

#define WEB3 "https://writernote-b.ddns.net/3_m"
void cloud_ui::on_ac_3_clicked()
{
    mostra_finestra_i(WEB3);
}

#define WEB6 "https://writernote-b.ddns.net/6_m"
void cloud_ui::on_ac_6_clicked()
{
    mostra_finestra_i(WEB6);
}

#define WEB12 "https://writernote-b.ddns.net/6_m"
void cloud_ui::on_ac_12_clicked()
{
    mostra_finestra_i(WEB12);
}

/*
 * ok button
*/
void cloud_ui::on_button_ok_clicked()
{
    if(ui->button_log->isChecked()){
        if(ui->button_log->text() == TEXT_LOG_IN){
            /*
             * he need to log in
            */

            int pass = checkpassword(ui->edit_pass->toPlainText().toUtf8().constData(), ui->edit_repeat->toPlainText().toUtf8().constData());

            if(pass == PASS_LEN){
                user_message(MESSAGE_PASSWORD_NOT_LENGTH + (QString)MINSTRLEN);

            }else if (pass == PASS_NOT_S) {
                return user_message("Passwords do not match");

            } else if(pass == PASS_N){
                return user_message(MESSAGE_PASSWORD_NOT_M + (QString)MIN_NUMN + " uppercase");

            } else if(pass == PASS_M){
                return user_message(MESSAGE_PASSWORD_NOT_M + (QString)MIN_NUMM + " number");

            }


            struct_user *temp_user = new struct struct_user;

            strcpy(temp_user->m_mail.m_mail, ui->edit_mail->toPlainText().toUtf8().constData());
            strcpy(temp_user->password, ui->edit_pass->toPlainText().toUtf8().constData());

            auto res = this->m_controll->action(n_request::login_user, temp_user);

            if(res == n_error_cloud::error_no_internet)
                return user_message(MESSAGE_NOT_INTERNET);
            else if(res == n_error_cloud::server_down)
                return user_message(MESSAGE_SERVER_DOWN);


        }else if(ui->button_log->text() == TEXT_LOG_ON){
            /*
             * he need to delete user from qsetting and clear user from cloud_controll
            */
            this->m_controll->cleanUser();

            ui->button_log->setText(TEXT_LOG_IN);
            ui->button_register->setText("Register");

        }
    }else if(ui->button_register->isChecked()){
        /* register new user */
        struct struct_user *temp_user = new struct struct_user;



    }
}
