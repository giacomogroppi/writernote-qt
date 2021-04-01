#include "extract_audio_ui.h"
#include "ui_extract_audio_ui.h"

#include "extract_audio.h"
#include "QFile"
#include "../dialog_critic/dialog_critic.h"

#include <QDebug>
#include "../../datawrite/qfilechoose.h"

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

    auto res = extract_audio(ui->edit_path->toPlainText().TOUTF,
                            ui->comboBox->currentText().TOUTF,
                            ui->to_exit->toPlainText().TOUTF);

    switch (res) {
    case extract::ok:
        return messaggio_utente("Extraction complite to " + ui->edit_path->toPlainText());

    case extract::load_audio:
        return dialog_critic("I had a problem reading the audio");

    case extract::no_copybook:
        /*
         * the function was created to work in main, and opens
         * the index of its own to check that this copybook
         * exists, if it returns xxx, it means that the data has
         * changed since we originally read it, to now, or
         * there was an opening error
        */
        return dialog_critic("I had an internal problem, please retry");

    case extract::not_record:
        /*
         * TODO: -> find a way to warn the user first of
         * the fact that and the copybook is not registered
        */
        return messaggio_utente(ui->comboBox->currentText() + " is not recorder");
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
    qfilechoose m_file (nullptr);
    if(!m_file.filechoose(&temp_path, TYPEAUDIO))
        return;

    ui->to_exit->setPlainText(temp_path);

}

/*
 * open writernote file
*/
#if defined(WIN32) || defined(WIN64) || defined(OS2)
#define HOME "USERPROFILE"
#elif unix
#define HOME "HOME"
#endif

void extract_audio_ui::on_open_from_clicked()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this, tr("Open File"), getenv(HOME), "Writernote (*.writer);; All file (* *.*)");

    if(fileName.isEmpty())
        return;

    ui->edit_path->setPlainText(fileName);

}
