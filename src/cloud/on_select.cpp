#include "cloud_ui.h"
#include "ui_cloud_ui.h"

#include "../utils/dialog_critic/dialog_critic.h"

void cloud_ui::on_tab_main_currentChanged(int)
{
    this->refresh();
}


#define PASS_HIDE "**************"
/*
 * temp = true -> show log in
 * temp = false -> show lon on
*/
static void show_log_in(cloud_ui *cloud, bool temp, struct struct_user *user){
    if(temp)
        cloud->ui->button_log->setText("Log in");
    else
        cloud->ui->button_log->setText("Log on");

    cloud->ui->button_pass->setHidden(temp);

    if(user != NULL){
        cloud->ui->edit_mail->setPlainText((QString)user->m_mail.m_mail);
        cloud->ui->edit_pass->setPlainText(PASS_HIDE);
    }

    cloud->ui->edit_repeat->setHidden(!temp);
}



/*
 * redo the log in to find if the user is log
 * ask for balance
*/
void cloud_ui::refresh(){
    void *pointer;
    auto res = m_controll->action(n_request::balance, &pointer);

    if(res != n_error_cloud::ok){
        if(res == n_error_cloud::password_wrong){
            dialog_critic("Your password is wronge");

            show_log_in(this, true, m_controll->m_user);

        }else if(res == n_error_cloud::server_down){
            dialog_critic("Unfortunately i can't connect with the server");

        }else if(res == n_error_cloud::not_login){
            /*
             * need to show log in button
            */
            show_log_in(this, true, m_controll->m_user);

            ui->balance->setHidden(true);

        }else if(res == n_error_cloud::error_no_internet){
            messaggio_utente("It seems you don't have internet");
        }

    }else{
        show_log_in(this, false, m_controll->m_user);

        ui->balance->setText(QString::number(* (int*) pointer));
    }
}
