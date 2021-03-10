#include "retry_ui.h"
#include "ui_retry_ui.h"

#include <QCloseEvent>

retry_ui::retry_ui(QWidget *parent, QString title, QString message, QString error) :
    QDialog(parent),
    ui(new Ui::retry_ui)
{
    ui->setupUi(this);

    ui->text->setText(message);
    this->setWindowTitle(title);

    this->message = message;
    this->title = title;
    this->messageError = error;
}

retry_ui::~retry_ui()
{
    delete ui;
}

/*
 * if result is ok
*/
void retry_ui::resultRetry(bool check)
{
    QString temp = this->message + "\n" + this->messageError;
    this->ui->text->setText(temp);
}

void retry_ui::on_button_retry_clicked()
{
    emit retry();
}

void retry_ui::on_button_close_clicked()
{
    emit close();
}

void retry_ui::closeEvent(QCloseEvent *event)
{
    emit close();
    return event->accept();
}
