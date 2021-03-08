#include "extract_audio_ui.h"
#include "ui_extract_audio_ui.h"

#include "extract_audio.h"
#include "QFile"
#include "../dialog_critic/dialog_critic.h"

#include <QDebug>

#define TOUTF toUtf8().constData()

extract_audio_ui::extract_audio_ui(QWidget *parent, indice_class *indice, QString *path) :
    QDialog(parent),
    m_indice(indice),

    ui(new Ui::extract_audio_ui)
{
    ui->setupUi(this);

    if(this->m_indice != nullptr){
        this->ui->edit_path->setPlainText(*path);
        this->ext = false;
    }



}

void extract_audio_ui::setData(){
    int i, len;
    len = m_indice->titolo.length();
    for(i=0; i<len; i++){
        this->ui->comboBox->addItem(m_indice->titolo.at(i));
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
            QFile file(ui->edit_path->toPlainText().TOUTF);
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
        if(m_indice)
            delete m_indice;
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
    QFile file_temp = QFile(ui->edit_path->toPlainText().TOUTF);
    if(!file_temp.exists())
        return dialog_critic("The file didn't exist");

    int res = extract_audio(ui->edit_path->toPlainText().TOUTF,
                            ui->comboBox->currentText().TOUTF,
                            ui->to_exit->toPlainText().TOUTF);



}

void extract_audio_ui::on_edit_path_textChanged()
{
    this->iswriting = true;
}
