#include "bug_repo_ui.h"
#include "ui_bug_repo_ui.h"

#include "../../mainwindow.h"
#include "../../windows/mostra_finestra_i.h"
#include "../copy_text.h"

bug_repo_ui::bug_repo_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bug_repo_ui)
{
    ui->setupUi(this);

    ui->label->setText(ISSUE_MESSAGE);
    ui->label_status->setText(NONE);
}

bug_repo_ui::~bug_repo_ui()
{
    delete ui;
}

void MainWindow::on_actionBug_report_triggered()
{
    bug_repo_ui temp;
    temp.exec();

}



void bug_repo_ui::on_button_copy_mail_clicked()
{
    copy_text(MAIL);

    ui->label_status->setText("Copied");

}

void bug_repo_ui::on_button_open_github_clicked()
{
    mostra_finestra_i(URL_ISSUE_GITHUB);
    ui->label_status->setText(NONE);
}
