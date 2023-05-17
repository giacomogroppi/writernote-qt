#include "cloud_ui.h"

#ifdef CLOUD

void cloud_ui::on_tab_main_currentChanged(int index)
{
    this->refresh();

    if(index == POSITION_LOG){
        this->on_button_log_clicked();
    }
}


#define PASS_HIDE "**************"
/*
 * temp = true -> show log in
 * temp = false -> show lon on
*/
static void show_log_in(cloud_ui *cloud, bool temp, struct struct_user *user){
    cloud->ui->button_register->setHidden(!temp);

    cloud->ui->button_register->setCheckable(temp);
    cloud->ui->button_log->setCheckable(temp);

    if(temp){
        cloud->setWindowTitle((WString)NAME + " - log in");
        cloud->ui->button_register->setChecked(false);
        cloud->ui->button_log->setChecked(true);

        cloud->ui->button_log->setText(TEXT_LOG_IN);
    }
    else
        cloud->ui->button_log->setText(TEXT_LOG_ON);

    cloud->ui->button_pass->setHidden(temp);

    if(user != NULL){
        cloud->ui->edit_mail->setPlainText((WString)user->m_mail.m_mail);
        cloud->ui->edit_pass->setPlainText(PASS_HIDE);
    }

    cloud->ui->edit_repeat->setHidden(!temp);
}


/*
 * redo the log in to find if the user is log
 * ask for balance
*/
void cloud_ui::refresh(){
    auto res = m_controll->action(n_request::balance);

    if(res != n_error_cloud::ok){
        if(res == n_error_cloud::wait)
            return; /*TODO*/

        if(res == n_error_cloud::password_wrong){
            dialog_critic("Your password is wrong");

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
            user_message("It seems you don't have internet");
        }

    }
}


void cloud_ui::on_button_log_clicked()
{
    this->ui->button_log->setChecked(true);
    this->ui->button_register->setChecked(false);

    this->ui->edit_repeat->setHidden(true);
    this->ui->label_repeat->setHidden(true);
}

void cloud_ui::on_button_register_clicked()
{
    this->ui->button_log->setChecked(false);
    this->ui->button_register->setChecked(true);

    this->setWindowTitle((WString)NAME + " - register");
    this->ui->edit_repeat->setHidden(false);
    ui->label_repeat->setHidden(false);
}

#endif
