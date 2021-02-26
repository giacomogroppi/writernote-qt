#include "cloud_ui.h"
#include "ui_cloud_ui.h"

#include "../utils/dialog_critic/dialog_critic.h"

void cloud_ui::on_tab_main_currentChanged(int)
{
    this->refresh();
}

/*
 * temp = true -> show log in
 * temp = false -> show lon on
*/
static void show_log_in(cloud_ui *cloud, bool temp){
    if(temp)
        cloud->ui->button_info->setText("Log in");
    else
        cloud->ui->button_log->setText("Log on");
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

            show_log_in(this, true);

        }else if(res == n_error_cloud::server_down){
            dialog_critic("Unfortunately i can't connect with the server");

        }else if(res == n_error_cloud::not_login){
            /*
             * need to show log in button
            */
            show_log_in(this, true);

            /* setting balance to hide */
            ui->balance->setHidden(true);

        }else if(res == n_error_cloud::error_no_internet){
            messaggio_utente("It seems you don't have internet");
        }

    }else{
        show_log_in(this, false);

        ui->balance->setText(QString::number(* (int*) pointer));
    }
}
