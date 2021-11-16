#include "extract_audio_ui.h"
#include "ui_extract_audio_ui.h"
#include "../../datawrite/qfilechoose.h"
#include "extract_audio.h"
#include "QFile"
#include "../dialog_critic/dialog_critic.h"
#include <QFileDialog>
#include <QDebug>
#include "../../datawrite/qfilechoose.h"

extract_audio_ui::extract_audio_ui(QWidget *parent, const QString *path) :
    QDialog(parent),
    ui(new Ui::extract_audio_ui)
{
    ui->setupUi(this);

    if(path){
        this->ui->edit_path->setPlainText(*path);
        this->ext = false;
    }
}

/*
 * change the color of edit_path
 * red if the file didn't exist
*/
void extract_audio_ui::ChangeColor(QColor color){
    ui->edit_path->setAutoFillBackground(true);
    QPalette pal = ui->edit_path->palette();

    pal.setColor(QPalette::Base, color);

    ui->edit_path->setPalette(pal);

    ui->edit_path->update();
}

bool extract_audio_ui::event(QEvent *event)
{

    if(this->iswriting){
        /*
         * MouseButtonPress is call only when the user
         * click outside the box
        */
        if(event->type() == QEvent::MouseButtonPress && !ui->edit_path->toPlainText().isEmpty()){
            QFile file(ui->edit_path->toPlainText());
            if(file.exists()){
                this->ChangeColor(Qt::green);
            }
            else{
                this->ChangeColor(Qt::red);
            }
        }
        this->iswriting = false;
    }

    return QWidget::event(event);
}

extract_audio_ui::~extract_audio_ui()
{
    delete ui;

    if(this->ext){
        if(m_current)
            delete m_current;
    }
}

void extract_audio_ui::on_button_close_clicked()
{
    this->close();
}

void extract_audio_ui::on_extract_to_clicked()
{
    QFile file_temp(ui->edit_path->toPlainText());
    if(!file_temp.exists())
        return dialog_critic("The file didn't exist");

    auto res = extract_audio(ui->edit_path->toPlainText(),
                            ui->to_exit->toPlainText());

    switch (res) {
    case extract::ok:
        return user_message("Extraction complite to " + ui->edit_path->toPlainText());

    case extract::load_audio:
        return dialog_critic("I had a problem reading the audio");

    case extract::not_record:
        /*
         * TODO: -> find a way to warn the user first of
         * the fact that and the copybook is not registered
        */
        return user_message("This file is not recorder");
    case extract::open_to:
        return dialog_critic("I had a problem saving the file in " + ui->to_exit->toPlainText());

    case extract::load_file:
        return dialog_critic("We had a problem uploading the file");
    default:
        break;
    }

}

void extract_audio_ui::on_edit_path_textChanged()
{
    this->iswriting = true;
}

/*
 * open file to
*/
void extract_audio_ui::on_open_to_clicked()
{
    QString temp_path;
    if(!qfilechoose::getFileForLoad(temp_path, TYPEAUDIO))
        return;

    ui->to_exit->setPlainText(temp_path);

}

void extract_audio_ui::on_open_from_clicked()
{
    QString fileName;

    if(!qfilechoose::getFileForLoad(fileName, TYPEFILEWRITER))
        return;

    ui->edit_path->setPlainText(fileName);

}
