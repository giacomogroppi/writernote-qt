#include "cloud_ui.h"
#include "ui_cloud_ui.h"
#include <QMessageBox>
#include "../windows/mostra_finestra_i.h"
#include "../utils/dialog_critic/dialog_critic.h"

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
            messaggio_utente("Password wrong");
        }else if(return_command == n_error_cloud::ok){
            save_recent_user(m_controll->m_user);
        }
    }else if(last_command == n_request::register_user){
        if(return_command == n_error_cloud::server_down)
            return serverDown();

        if(return_command == n_error_cloud::ok){
            messaggio_utente("Successful registration");
        }else{
            messaggio_utente("Something went wrong");
        }
    }
}

void cloud_ui::serverDown(){
    messaggio_utente("Unfortunately the server is not reachable, please try again in some time");
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


