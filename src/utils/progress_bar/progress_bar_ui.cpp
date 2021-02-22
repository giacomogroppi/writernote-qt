#include "progress_bar_ui.h"
#include "ui_progress_bar_ui.h"

#define MAX 100
#define MIN 0

progress_bar_ui::progress_bar_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progress_bar_ui)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(MAX);
}

progress_bar_ui::~progress_bar_ui()
{
    delete ui;
}

void progress_bar_ui::finished_()
{
    this->close();
}

/*
 * progress > 0
 * progress < 100
*/
void progress_bar_ui::progress_(int progress)
{
    ui->progressBar->setValue(progress);
}


/* close */
void progress_bar_ui::on_close_button_clicked()
{
    emit closeForce();
}
