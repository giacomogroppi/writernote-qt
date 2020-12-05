#include "videocompress_ui.h"
#include "ui_videocompress_ui.h"

#include "../mainwindow.h"

#include "enable_videocompress.h"

videocompress_ui::videocompress_ui(QWidget *parent, MainWindow *padre) :
    QDialog(parent),
    ui(new Ui::videocompress_ui)
{
    ui->setupUi(this);
    this->padre = padre;
    enable_videocompress_ui(this->padre, this);

}

videocompress_ui::~videocompress_ui()
{
    delete ui;
}

void videocompress_ui::on_pushButton_clicked()
{
    this->close();
}

void posizionefile(videocompress_ui *parent, bool posizione){
    /* true per from */

    QString fileName = QFileDialog::getOpenFileName(parent, "Open File", "/home/giacomo", "Video (*.wav, *.avi, *.mp4, *.flv, *.mkv)");

    if(posizione){
        parent->posizionefrom = fileName;
        parent->ui->plainTextEdit_from->setPlainText(fileName);
    }
    else{
        parent->posizionefrom = fileName;
        parent->ui->textedit_destinazione->setPlainText(fileName);
    }
}

/* bottone per il from */

void videocompress_ui::on_bottonefrom_clicked()
{
    posizionefile(this, true);
}


/* bottone per to */
void videocompress_ui::on_pushButton_4_clicked()
{
    posizionefile(this, false);
}



static void settaggiobottoneok(videocompress_ui *parent){
    if(parent->ui->plainTextEdit_from->toPlainText() != "" && parent->ui->textedit_destinazione->toPlainText() != "")
        parent->ui->pushButton_ok->setEnabled(true);
    else
        parent->ui->pushButton_ok->setEnabled(false);
}

void videocompress_ui::on_plainTextEdit_from_textChanged()
{
    if(this->ui->plainTextEdit_from->toPlainText() != "")
        this->ui->plainTextEdit_from->setStyleSheet("QPlainTextEdit { background-color : #d9d9d9; }");
    else
        this->ui->plainTextEdit_from->setStyleSheet("QPlainTextEdit { background-color : #ffffff; }");
    settaggiobottoneok(this);
}

void videocompress_ui::on_textedit_destinazione_textChanged()
{
    if(this->ui->textedit_destinazione->toPlainText() != "")
        this->ui->textedit_destinazione->setStyleSheet("QPlainTextEdit { background-color : #d9d9d9; }");
    else
        this->ui->textedit_destinazione->setStyleSheet("QPlainTextEdit { background-color : #ffffff; }");

    settaggiobottoneok(this);
}



