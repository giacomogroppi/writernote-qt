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

    void *pointer;
    auto res = m_controll->action(n_request::balance, &pointer);
    if(res != n_error_cloud::ok){
        if(res == n_error_cloud::password_wrong){
            dialog_critic("Your password is wronge");
        }else if(res == n_error_cloud::server_down){
            dialog_critic("Unfortunately i can't connect with the server");
        }

    }else{
        ui->balance->setText(QString::number(* (int*) pointer));
    }
}

cloud_ui::~cloud_ui()
{
    delete ui;
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
