#include "ui_restore.h"
#include "restore.h"
#include "utils/get_file_dir/get_file_dir.h"
#include "utils/common_def.h"
#include <QDir>
#include <QFileDialog>
#include "utils/slash/slash.h"
#include "restore_file/restore_file_critic.h"
#include "restore_file/get_name_tmp.h"
#include "utils/dialog_critic/dialog_critic.h"
#include "testing/memtest.h"

restore::restore(QWidget *parent, QString path) :
    QDialog(parent),
    ui(new Ui::restore)
{
    ui->setupUi(this);

    ui->message_label->setText("");

    WNew(m_curr, Document, ());

    this->path = path;

    ui->plainTextEdit->setPlainText(path);

    updateList();
}

restore::~restore()
{
    WDelete(m_curr);

    if(m_save)
        delete m_save;
    if(m_xml)
        delete m_xml;

    delete ui;
}

void restore::on_close_button_clicked()
{
    this->close();
}

void restore::on_pushButton_open_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                "",
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);

    if(dir == ""){
        return;
    }

    path = dir;

    __l = get_file_dir::get(path);

    updateList();

}

void restore::removeNotWriternote(QStringList &l)
{
    uint i, len;
    QStringList list;
    QString tmp;

    len = l.length();

    for(i=0; i<len; ++i){
        list = l.at(i).split(slash::__slash());
        QString &string = (QString &)list.last();

        if(string.indexOf(APP_EXT) == -1){
            l.removeAt(i);
            --len;
            --i;
            continue;
        }

        tmp = get_name_tmp::get(l.at(i));

        if(!QFile::exists(tmp)){
            l.removeAt(i);
            --len;
            --i;
        }

    }

}

void restore::updateList()
{
    uint i, len;

    ui->plainTextEdit->setPlainText(path);

    removeNotWriternote(__l);

    len = __l.length();
    ui->listWidget->clear();

    for(i=0; i<len; ++i){
        ui->listWidget->addItem(__l.at(i));
    }

    if(!len){
        if(!ui->listWidget->count()){
            ui->message_label->setText("There is no file to restore");
        }
    }

}

void restore::on_ok_restore_clicked()
{
    QString pos_res;
    QDir __dir(path);
    int index;
    restore_file_critic::n_err __res;

    if(path != "" && path.indexOf(APP_EXT) != -1){
        if(!QFile::exists(get_name_tmp::get(path))){
            ui->message_label->setText("In " + path + " I can't find a restore file");
            return;
        }

        pos_res = path;

    }else if(!__dir.exists()){
        ui->message_label->setText("Select a file before restore any file");
    }else{
        index = ui->listWidget->currentRow();
        if(index == -1){
            if(ui->listWidget->count() > 0){
                ui->message_label->setText("Select a file in the list");
            }
            return;
        }else{
            pos_res = ui->listWidget->item(index)->text();
        }
    }

    __res = restore_file_critic::restore_file(pos_res);

    if(__res == restore_file_critic::restore_ok){
        user_message("Copybook restore");
    }else{
        dialog_critic("We had a problem restoring the file");
    }
}
